//#include <Wire.h>
#include "Constants.h"
#include "Sensors.h"
#include "Resources.h"
#include "Connection.h"

// pointers for call object
Sensors sensor;
Resources presc__;
Connection connection;

boolean sendData = false;

// sensor's variables
float temperature;
float humidity;
int _valueLDR;
int _valLDRmin = 1023;
int _valLDRmax = 0;
  
float _valueUV;
float _sound;
float _flowmeter; 
float _volume ;
float _NO2;
float _CO2;

long value_sensors[SENSORS_NUMBER];

volatile uint16_t pulses;

void ISRFuncFlowmeter(){
  pulses++;  
}


float Sensors::getFlowMeter(){
  
  pulses = 0;  
  sei();     
  delay (1000);   //Wait 1 second
  cli();      
  //_flowmeter = ((NbTopsFanSen * 60 / 5.5)/60); //(Pulse frequency x 60) / 5.5Q, = flow rate in L/hour 
  _flowmeter = (pulses/7.5)*60; //(Pulse frequency x 60) / 7.5Q, = flow rate 
  _volume = (_flowmeter * 1); // m3/min
 //if(flat == 1) return convertF2C(_flowmeter);
 //else return convertF2C(_volume);
}

void Sensors::begin(){ //init variables

  Serial.begin(115200);//9600
  Serial1.begin(9600);//9600
  /*
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
    
  } */

  
  delay(2000);
  connection.begin();
  presc__.begin(); 
  
#if DEBUG_MODE
      Serial.println(F("DEBUG")); 
#endif 

#if ENABLED_INT_FLOWMETER  
  pinMode(FLOWMETER_PIN, INPUT); //initializes digital pin 2 as an input (Flowmeter)
  attachInterrupt(INT0, ISRFuncFlowmeter, RISING); 
#endif
  
}

void Sensors::readSerials(){
  
  if(Serial.available()){
    byte inByte = Serial.read();
    
  }
  
  if (Serial1.available()) {
          byte inByte = Serial1.read();
          Serial.write(inByte);
  } 
}

ISR(TIMER1_OVF_vect){
  sensor.readSerials();  
}

void Sensors::execute(){ // init program

#if ENABLED_INT_FLOWMETER  
  getFlowMeter();
#endif

  sensorsUpdate();
  
  if(!connection.wifiStatus)
    connection.attachWIFI();  
    
  if(!connection.httpPOST(SERVER, PORT, value_sensors) && !connection.wifiStatus){
#if DEBUG_MODE
      Serial.print(F("Can't connect to remote server ")); 
      Serial.print(SERVER);
      Serial.print(F(" at port "));
      Serial.println(PORT);  
#endif
  }
  else{
#if DEBUG_MODE
    Serial.println(F("Please be sure is connected")); 
#endif
    if(connection.connStatus)
     connection.disconnectTCP();
  }
  
  delay(3000);

}

uint8_t Sensors::readDataDHT()
{
	// BUFFER TO RECEIVE
	uint8_t bits[5];
	uint8_t cnt = 7;
	uint8_t idx = 0;

	// EMPTY BUFFER
	for (int i=0; i< 5; i++) bits[i] = 0;

	// REQUEST SAMPLE
	pinMode(DHT_PIN, OUTPUT);
	digitalWrite(DHT_PIN, LOW);
	delay(18);
	digitalWrite(DHT_PIN, HIGH);
	delayMicroseconds(40);
	pinMode(DHT_PIN, INPUT);

	// ACKNOWLEDGE or TIMEOUT
	unsigned int loopCnt = 10000;
	while(digitalRead(DHT_PIN) == LOW)
		if (loopCnt-- == 0) return -2; //-2 : timeout

	loopCnt = 10000;
	while(digitalRead(DHT_PIN) == HIGH)
		if (loopCnt-- == 0) return -2; //-2 : timeout

	// READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT
	for (int i=0; i<40; i++)
	{
		loopCnt = 10000;
		while(digitalRead(DHT_PIN) == LOW)
			if (loopCnt-- == 0) return -2; //-2 : timeout

		unsigned long t = micros();

		loopCnt = 10000;
		while(digitalRead(DHT_PIN) == HIGH)
			if (loopCnt-- == 0) return -2; //-2 : timeout

		if ((micros() - t) > 40) bits[idx] |= (1 << cnt);
		if (cnt == 0)   // next byte?
		{
			cnt = 7;    // restart at MSB
			idx++;      // next byte!
		}
		else cnt--;
	}

	// WRITE TO RIGHT VARS
// as bits[1] and bits[3] are allways zero they are omitted in formulas.
	humidity    = (bits[0] + bits[1])/10; 
	temperature = (bits[2] + bits[3])/10; 

	uint8_t sum = bits[0] + bits[1] + bits[2] + bits[3];  

	if (bits[4] != sum) return -1; // -1 : checksum error
	return 0; //  0 : OK
}

float Sensors::getLDR(){
  _valueLDR = analogRead(LDR_PIN);
  
  if(_valueLDR < 5) return 0.0;
  
  if(_valueLDR>=5 && _valueLDR < 36){
    _valueLDR = map(_valueLDR, 5, 36, 1, 10);
    _valueLDR = _valueLDR/10;
  }else if(_valueLDR >= 36 && _valueLDR < 215){
    _valueLDR = map(_valueLDR, 36, 215, 1, 10);
    
  }else if(_valueLDR >= 215 && _valueLDR < 477){
    _valueLDR = map(_valueLDR, 215, 477, 10, 40);
  }else if(_valueLDR >= 477 && _valueLDR < 673){
    _valueLDR = map(_valueLDR, 477, 673, 40, 100);
  }else if(_valueLDR >= 673 && _valueLDR < 795){
    _valueLDR = map(_valueLDR, 673, 795, 100, 200);
  }else if(_valueLDR >= 795 && _valueLDR < 955){
    _valueLDR = map(_valueLDR, 795, 955, 200, 1000);
  }else
    _valueLDR = map(_valueLDR, 955, 1023, 100, 10000);
    
  return _valueLDR;
}

float Sensors::getUV(){
  _valueUV = analogRead(UV_PIN);
  return _valueUV;

}

float Sensors::getNoise(){
  _sound = analogRead(SOUND_PIN);
  
  if(_sound < 2)
    _sound = map(_sound, 0, 2, 1, 2);
  else 
    _sound = map(_sound, 3, 1023, 3, 1023);  
  
  _sound = 20*log10(_sound);
 
  return _sound;
}

float Sensors::getCO2(){ 
  _CO2 = analogRead(CO2_PIN);
  return _CO2;
}

float  Sensors::getNO2(){ 
  _NO2 = analogRead(NO2_PIN);
  _NO2 = map(_NO2, 0, 1023, 0, 1000);
  return _NO2;

}

void Sensors::sensorsUpdate(){

  uint8_t check = readDataDHT();
  
  presc__.RTCread();
  value_sensors[0] = temperature;
  value_sensors[1] = humidity;   
  value_sensors[2] = getLDR();
  value_sensors[3] = getUV();
  value_sensors[4] = getNoise();
  
#if ENABLED_INT_FLOWMETER  
  value_sensors[5] = _flowmeter;
  value_sensors[6] = _volume; 
#else
  value_sensors[5] = -1.0;
  value_sensors[6] = -1.0;
#endif  

  value_sensors[7] = getNO2();
  value_sensors[8] = getCO2(); 

#if HAS_GPS
  long latLgt[3];
  latLgt[0] = 1.0;
  latLgt[1] = 1.0;
  
  //getLatLgt(latLgt);
  value_sensors[9] = latLgt[0];
  value_sensors[10] = latLgt[1];
#else
  value_sensors[9] = -1.0;
  value_sensors[10] = -1.0;
#endif
  
}

