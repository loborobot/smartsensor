//#include <Wire.h>
#include "Constants.h"
#include "Sensors.h"
#include "Core.h"
//#include "Connection.h"
#include <WiFlyHQ.h>
// pointers for call object
Sensors sensor;
//Resources presc__;
//Connection connection;
WiFly wifly;

char buf[32];
// sensor's variables
float temperature;
float humidity;
float liters;
boolean wifiStatus;
boolean connStatus;
byte tried;

long value_sensors[SENSORS_NUMBER];
char current_time[20];

// count how many pulses!
volatile uint16_t pulses = 0;
// track the state of the pulse pin
volatile uint8_t lastflowpinstate;
// you can try to keep time of how long it is between pulses
volatile uint32_t lastflowratetimer = 0;
// and use that to calculate a flow rate
volatile float flowrate;

Core core;

void ISRFuncFlowmeter(){
  pulses++;  
}

void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
  }
}
/*
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
}*/


void Sensors::begin(){ //init variables

  Serial.begin(DEFAULT_BAUD1);
  Serial1.begin(DEFAULT_BAUD2);

#ifdef DEBUG_MODE
  while(!Serial)
    Serial.println(F("Serial is ready "));
#endif      
  //connection.begin();
  
  if (!wifly.begin(&Serial1, &Serial)) {
#ifdef DEBUG_MODE     
    Serial.println(F("Failed to start wifly"));
#endif
    terminal();
    //readSerials();
  }
  
  if (!wifly.isAssociated()) 
  {
#ifdef DEBUG_MODE   
    Serial.println(F("Joining network"));
#endif
	wifly.setSSID(ssid);
	wifly.setPassphrase(pass);
	wifly.enableDHCP();
  
  	if (!wifly.join()) {
#ifdef DEBUG_MODE   
            Serial.println(F("Failed to join wifi network"));
#endif
  	    terminal();
            //readSerials();
  	}
  
  } else {
#ifdef DEBUG_MODE     
      Serial.println(F("Already joined network"));
#endif
  }

#ifdef DEBUG_MODE   
  Serial.print(F("IP: "));
  Serial.println(wifly.getIP(buf, sizeof(buf)));
#endif   

  if (wifly.isConnected()) {
    wifly.close();
  }

  wifiStatus = false;
  connStatus = false;
  tried = 0;
  
  core.begin(); 

  pinMode(CO2_PIN, INPUT);
  pinMode(NO2_PIN, INPUT);

#ifdef ENABLED_INT_FLOWMETER 
  pinMode(FLOWMETER_PIN, INPUT);
  digitalWrite(FLOWMETER_PIN, HIGH);
  lastflowpinstate = digitalRead(FLOWMETER_PIN);
  useInterrupt(true);
  //pinMode(FLOWMETER_PIN, INPUT); //initializes digital pin 2 as an input (Flowmeter)
  //attachInterrupt(INT0, ISRFuncFlowmeter, RISING); 
#endif
  
}

/* Connect the WiFly serial to the serial monitor. */
void Sensors::terminal()
{
    while (1) {
	if (wifly.available() > 0) {
	    Serial.write(wifly.read());
	}


	if (Serial.available() > 0) {
	    wifly.write(Serial.read());
	}
    }
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

ISR(TIMER0_COMPA_vect) {
  uint8_t x = digitalRead(FLOWMETER_PIN);
  
  if (x == lastflowpinstate) {
    lastflowratetimer++;
    return; // nothing changed!
  }
  
  if (x == HIGH) {
    //low to high transition!
    pulses++;
  }
  lastflowpinstate = x;
  flowrate = 1000.0;
  flowrate /= lastflowratetimer;  // in hertz
  lastflowratetimer = 0;
}

void Sensors::execute(){ // init program

#ifdef DEBUG_MODE    
  Serial.println("pass..");
#endif

 
  if (tried == 4 && !connStatus) 
  {
          
#ifdef DEBUG_MODE   
  
    Serial.println(F("Rebooting and joining network"));
#endif
        wifly.reboot();
        delay(2000);
        
	wifly.setSSID(ssid);
	wifly.setPassphrase(pass);
	wifly.enableDHCP();
  
  	if (!wifly.join()) {
#ifdef DEBUG_MODE   
            Serial.println(F("Failed to join wifi network"));
#endif
  	    terminal();
            //readSerials();
  	}
        tried = 0;
  }else {
#ifdef DEBUG_MODE     
      Serial.println(F("Already joined network"));
#endif
  }

  if (wifly.open(server, PORT)) 
   {
       sensorsUpdate();
#ifdef DEBUG_MODE     
      Serial.print(F(" Connected to "));
      Serial.println(server);
#endif      
      char itoaBuffer[8];
      byte i; 
     
      //char bodyPOST[]="{\"device\": \"166d77ac1b46a1ec38aa35ab7e628ab5\", \"pub_date\": \"2015-03-19T22:02:27.321Z\", \"temperature\": \"0\", \"humidity\": \"0\", \"light\": \"0\", \"ultra_violet\": \"0\", \"sound\": \"0\", \"flowmeter\": \"0\", \"volume\": \"0\", \"nitrogen_dioxide\": \"0\", \"carbon_monoxide\": \"0\", \"longitude\": \"1\", \"latitude\": \"1\"}";
#ifdef DEBUG_MODE
      for(i=0; i<5; i++)
          Serial.println(HTTPPOST[i]);
       
       Serial.print(HTTPPOST[5]);
       //itoa(strlen(bodyPOST),itoaBuffer,10);        
       //wifly.println(itoaBuffer);
       Serial.println("290");
       //Serial1.println(HTTPPOST[6]);
       Serial.println();
      
       for(i = 0; i<SENSORS_NUMBER; i++){ //11
        if(i==5)
        {
          Serial.print(bodyJSON3[i]);
          Serial.print(getFlowMeter());
          
        }else{
          Serial.print(bodyJSON3[i]);
          Serial.print(value_sensors[i]);
        }
       }
     
       Serial.print(bodyJSON3[i]);
       Serial.print(bodyJSON3[i+1]);
       Serial.print(core.getRTC());
       Serial.print(bodyJSON3[i+2]);
#endif

      for(i=0; i<5; i++)
          wifly.println(HTTPPOST[i]);
       
       wifly.print(HTTPPOST[5]);
       //itoa(strlen(bodyPOST),itoaBuffer,10);        
       //wifly.println(itoaBuffer);
       wifly.println("290");
       //Serial1.println(HTTPPOST[6]);
       wifly.println();
      
       for(i = 0; i<SENSORS_NUMBER; i++){ //11
        if(i==5)
        {
          wifly.print(bodyJSON3[i]);
          wifly.print(getFlowMeter());
          
        }else{
          wifly.print(bodyJSON3[i]);
          wifly.print(value_sensors[i]);
        }
       }
     
       wifly.print(bodyJSON3[i]);
       wifly.print(bodyJSON3[i+1]);
       wifly.print(core.getRTC());
       wifly.print(bodyJSON3[i+2]);
       
#ifdef DEBUG_MODE             
       Serial.print(F(" \nPosted to server pulses "));
       Serial.print(pulses);
       Serial.print(F(" flowmeter  "));
       Serial.print(getFlowMeter());
       
#endif             
      /*
      for(i=0; i<5; i++)
        Serial1.println(HTTPPOST[i]);
      
      wifly.print(HTTPPOST[5]);
      //itoa(strlen(bodyPOST),itoaBuffer,10);        
      wifly.println("300");
      Serial1.println();    
      wifly.print(bodyPOST);
      */
      /*
      Serial1.print(F("["));
      
      for(i = 0; i<SENSORS_NUMBER; i++){
        Serial1.print(bodyJSON3[i]);
        Serial1.print(value_sensors[i]);
      }
     
      Serial1.print(bodyJSON3[i]);
      Serial1.print(value_sensors[i]);
      Serial1.print(bodyJSON3[i+1]);
      Serial1.print(value_sensors[i+1]);
      Serial1.print(bodyJSON3[i+2]);
      Serial1.println(F("]"));
      Serial1.println();
      
      */
      
      
      wifly.close();
      
      
  } else {
    connStatus = false;
    tried++;
    
#ifdef DEBUG_MODE       
    Serial.println(F("Failed to connect"));
      
#endif
  }
  
        

/*  
  if(!connection.wifiStatus)
    connection.attachWIFI();  
  else
  {    
    if(!connection.httpPOST(SERVER, PORT, value_sensors)){
#if DEBUG_MODE
      Serial.print(F("Can't connect to remote server ")); 
      Serial.print(SERVER);
      Serial.print(F(" at port "));
      Serial.println(PORT);  
#endif
    }
    else
    {
#if DEBUG_MODE
      Serial.println(F("Please be sure is connected")); 
#endif
      if(connection.connStatus)
       connection.disconnectTCP();
    }
  }*/
  delay(60000);

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
  float _valueLDR = analogRead(LDR_PIN);
  
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
  return analogRead(UV_PIN);
}

float Sensors::getNoise(){
  float _sound = analogRead(SOUND_PIN);
  
  if(_sound < 2)
    _sound = map(_sound, 0, 2, 1, 2);
  else 
    _sound = map(_sound, 3, 1023, 3, 1023);  
  
  _sound = 20*log10(_sound);
 
  return _sound;
}

float Sensors::getCO2(){ 
  return analogRead(CO2_PIN);
}

float  Sensors::getNO2(){ 
  return map(analogRead(NO2_PIN), 0, 1023, 0, 1000);
}

float Sensors::getFlowMeter(){
  float liters = pulses;
  liters /= 7.5;
  liters /= 60.0;
  return liters;
}

void Sensors::sensorsUpdate(){

  uint8_t check = readDataDHT();
  
  //presc__.RTCread();
  value_sensors[0] = temperature;
  value_sensors[1] = humidity;   
  value_sensors[2] = getLDR();
  value_sensors[3] = getUV();
  value_sensors[4] = getNoise();
  
#ifdef ENABLED_INT_FLOWMETER  == 1
  
  value_sensors[5] = getFlowMeter();
  value_sensors[6] = 0.0; 
#else
  value_sensors[5] = -1.0;
  value_sensors[6] = -1.0;
#endif  

  value_sensors[7] = getNO2();
  value_sensors[8] = getCO2(); 

#ifdef HAS_GPS
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

