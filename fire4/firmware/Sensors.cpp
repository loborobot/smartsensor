//#include <Wire.h>
#include "Constants.h"
#include "Sensors.h"
#include "Resources.h"
#include "Connection.h"

// pointers for call object
Sensors psens__;
Resources presc__;
Connection pconn__;

boolean sendData = false;

// sensor's variables
float _temperature;
float _humidity;
int _valueLDR;
int _valLDRmin = 1023;
int _valLDRmax = 0;
  
float _valueUV;
float _sound;
float _flowmeter; 
float _volume ;
float _NO2;
float _CO2;

void Sensors::begin(){ //init variables
  Serial.println("Start Sensonrs Config"); 
  pconn__.begin();
  presc__.begin(); 
  
 
  // calibrate during the first five seconds 
  /*while (millis() < 5000) {
    _valueLDR = analogRead(LDR_PIN);
    // record the maximum sensor value
    if (_valueLDR > _valLDRmax) {_valLDRmax = _valueLDR;}
    // record the minimum sensor value
    if (_valueLDR < _valLDRmin) {_valLDRmin = _valueLDR;}
  }*/
}

void Sensors::execute(int flo, int vol){ // init program
  String data = strucData(flo, vol);
  //if(pconn__.statusConn==true && pconn__.statusServer==true){
    //while(flat == -1){updateSensor();}
    //if(flat == 0){
  pconn__.printData(data);
  if(pconn__.sendQueryData(data)){
    sendData = true;
  }
  else{
    //pconn__.connect();
    sendData = false;
  }
    //}
  //}
  //else presc__.writeSD(); 
}

uint8_t Sensors::readDataDHT() // temperature and humidity function
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
	_humidity    = (bits[0] + bits[1])/10; 
	_temperature = (bits[2] + bits[3])/10; 

	uint8_t sum = bits[0] + bits[1] + bits[2] + bits[3];  

	if (bits[4] != sum) return -1; // -1 : checksum error
	return 0; //  0 : OK
}

int Sensors::readDataLDR() // LDR function
{
  
  //Serial.print("temperature: ");Serial.println(_humidity);
  //Serial.print("humedad: ");Serial.println(_temperature);
  _valueLDR = analogRead(LDR_PIN);
  //Serial.print("ldr: ");Serial.println(_valueLDR);
  //_valueLDR = map(analogRead(LDR_PIN), _valLDRmin, _valLDRmax, 0, 255);
  //_valueLDR = constrain(_valueLDR, 0, 255);
  return _valueLDR;
  
  /*if(_valueLDR < 5) return 0.0;
  if(_valueLDR>=5 && _valueLDR <= 36){
    _valueLDR = map(_valueLDR, 5, 36, 1, 10);
    _valueLDR = _valueLDR/10;
    return 0;
  }
  if(_valueLDR>36 && _valueLDR <= 215){
    _valueLDR = map(_valueLDR, 36, 215, 1, 10);
    return 0; 
  }
  if(_valueLDR>215 && _valueLDR <= 477){
    _valueLDR = map(_valueLDR, 215, 477, 10, 40);
    return 0;
  }
  if(_valueLDR>477 && _valueLDR <= 673){
    _valueLDR = map(_valueLDR, 477, 673, 40, 100);
    return 0;
  }
  if (_valueLDR>673 && _valueLDR <= 795){
    _valueLDR = map(_valueLDR, 673, 795, 100, 200);
    return 0;
  }
  if(_valueLDR>795 && _valueLDR <= 955){
    _valueLDR = map(_valueLDR, 795, 955, 200, 1000);
    return 0;
  }
  if(_valueLDR>795 && _valueLDR <= 955){
    _valueLDR = map(_valueLDR, 795, 955, 200, 1000);
    return 0;
  }
  if(_valueLDR > 1000) return -1;*/
}

char* Sensors::readDataUV(){ // UV function
  _valueUV = analogRead(UV_PIN);
  //Serial.print("uv: ");Serial.println(_valueUV);
  //_valueUV = map(analogRead(UV_PIN), 0, 30, 0, 15); // read pinUV
  return convertF2C(_valueUV);
}

char* Sensors::readDataSound(){
  _sound = analogRead(SOUND_PIN);
  //Serial.print("sonido: ");Serial.println(_sound);
 // _sound = map(analogRead(SOUND_PIN), 0, 1023, 0, 100); //Sound
 return convertF2C(_sound);
}

char* Sensors::readDataFlowmeter(int flat){
  
  NbTopsFanSen = 0;   //Set NbTops to 0 ready for calculations
  sei();      //Enables interrupts
  delay (1000);   //Wait 1 second
  cli();      //Disable interrupts
  _flowmeter = ((NbTopsFanSen * 60 / 5.5)/60); //(Pulse frequency x 60) / 5.5Q, = flow rate in L/hour 
  _volume = (_flowmeter * 1); // m3/min
 if(flat == 1) return convertF2C(_flowmeter);
 else return convertF2C(_volume);
}

char* Sensors::readDataCO2(){ //cos2 function
  _CO2 = analogRead(CO2_PIN);
  return convertF2C(_CO2);
}

char* Sensors::readDataNO2(){ //no2 funcion
  _NO2 = analogRead(NO2_PIN);
  return convertF2C(_NO2);
}

// strtc data function
String Sensors::strucData(int flo, int vol){
  int i = 0;
  String data;
  
  uint8_t check = readDataDHT();
  
  data += bodyJSON[i];
  data += bodyJSON[i+1];
  data += presc__.RTCread();
  data += bodyJSON[i+2];
  data += convertF2C(_temperature);
  data += bodyJSON[i+3];
  data += convertF2C(_humidity);
  data += bodyJSON[i+4];
  data += readDataLDR();
  data += bodyJSON[i+5];
  data += readDataUV();
  data += bodyJSON[i+6];
  data += readDataSound();
  data += bodyJSON[i+7];
  data += convertF2C(flo);
  data += bodyJSON[i+8];
  data += convertF2C(vol);
  data += bodyJSON[i+9];
  data += readDataNO2();
  data += bodyJSON[i+10];
  data += readDataCO2();
  data += bodyJSON[i+11];
  data += "-16.40370";
  data += bodyJSON[i+12];
  data += "-71.53064";
  data += bodyJSON[i+13];
  
  return data;
}

// convert float to string function
char* Sensors::convertF2C(float val){
  static char dtostrfbuffer[10];
  return dtostrf(val,5, 2, dtostrfbuffer);
}
