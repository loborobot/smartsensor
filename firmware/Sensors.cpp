//#include <Wire.h>
#include "Constants.h"
#include "Sensors.h"
#include "Core.h"
//#include "Connection.h"
#include <WiFlyHQ.h>
// pointers for call object
//Sensors sensor;
//Resources presc__;
//Connection connection;
WiFly wifly;

//char buf[32];
// sensor's variables
float temperature;
float humidity;
float liters;
boolean wifiStatus;
boolean connStatus;
byte tried;


uint8_t data[6];
int8_t _count;
unsigned long _lastreadtime;

boolean firstreading;

long value_sensors[SENSORS_NUMBER];
char current_time[20];
char buf[87];

// MEMORY: 27.336
//         27.654
    //     27.568
         //27.354  
// count how many pulses!
volatile uint16_t pulses = 0;
// track the state of the pulse pin
volatile uint8_t lastflowpinstate;
// you can try to keep time of how long it is between pulses
volatile uint32_t lastflowratetimer = 0;
// and use that to calculate a flow rate
volatile float flowrate;


//float vcc = 5.0;
//float Res0 = 100000;

Core core;


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

void send200()
{
    wifly.println(F("HTTP/1.1 200 OK"));
    wifly.println(F("Content-Type: text/html"));
    wifly.println(F("Transfer-Encoding: chunked"));
    wifly.println();

}

void send404()
{
    wifly.println(F("HTTP/1.1 404 Not Found"));
    wifly.println(F("Content-Type: text/html"));
    wifly.println(F("Transfer-Encoding: chunked"));
    wifly.println();
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

  Serial.begin(115200);
  Serial1.begin(9600);

  #ifdef DEBUG_MODE
    while(!Serial)
      Serial.println(F("Serial is ready "));
  #endif      
  //connection.begin();
  
  if (!wifly.begin(&Serial1, &Serial)) 
  {
  #ifdef DEBUG_MODE     
    Serial.println(F("Failed to start wifly"));
  #endif
    terminal();
    //readSerials();
  }
  
  wifiStatus = false;
  connStatus = false;
  tried = 0;
  
  if (!wifly.isAssociated()) 
  {
  #ifdef DEBUG_MODE   
    Serial.println(F("Joining network"));
  #endif
    wifly.setSSID(ssid);
    wifly.setPassphrase(pass);
    wifly.enableDHCP();
    wifly.save();
    
    if (wifly.join()) 
    {
      #ifdef DEBUG_MODE   
        Serial.println(F("Joined wifi network"));
      #endif
      
      wifiStatus = true; 
    
    }else
    {
      #ifdef DEBUG_MODE   
        Serial.println(F("Failed to join wifi network"));
      #endif
      wifly.terminal();
      //readSerials();  
      wifiStatus = false;
      
    }
  } else  
  {
    #ifdef DEBUG_MODE 
     Serial.println(F("Already joined network1"));
    #endif
  }
  
  wifly.setBroadcastInterval(0); // Turn off UPD broadcast
  
  #ifdef DEBUG_MODE   
    Serial.print(F("IP: "));
    Serial.println(wifly.getIP(buf, sizeof(buf)));
  #endif   

  wifly.setDeviceID(F("SmartSensor"));

  if (wifly.isConnected()) { //TCP connection
  #ifdef DEBUG_MODE   
    Serial.println(F("Old connection active. Closing"));
  #endif
    wifly.close();
    connStatus = false;
  }

  wifly.setProtocol(WIFLY_PROTOCOL_TCP);

  if (wifly.getPort() != 80) {
    wifly.setPort(80);
    /* local port does not take effect until the WiFly has rebooted (2.32) */
    wifly.save();
    #ifdef DEBUG_MODE   
      Serial.println(F("Set port to 80, rebooting to make it work"));
    #endif
    wifly.reboot();
    delay(3000);
  }
  
  core.begin(); 

  pinMode(DHT_PIN, INPUT);
  digitalWrite(DHT_PIN, HIGH);

  pinMode(CO_PIN, INPUT);
  pinMode(NO2_PIN, INPUT);

  firstreading=true;
  _lastreadtime=0;
  _count=6;

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
  
 unsigned long startTime = millis();
 unsigned long stopTime = startTime + POST_INTERVAL;
  
 
  while (millis() < stopTime){
    // wait for incomming data
    #ifdef DEBUG_MODE       
      //Serial.println(F(" waiting to next post")); 
    #endif     

    if(wifly.available() > 0) {
       /* See if there is a request */
      if (wifly.gets(buf, sizeof(buf))) 
      {
        Serial.println(buf);
        if (strncmp_P(buf, PSTR("POST /"), 6) == 0) 
        {
          byte i=0;
  	  while(i<9){
            wifly.gets(buf, sizeof(buf));
            Serial.println(buf);           
            
            //delay(10);
          }
          
	  send200();
          //wifly.reboot();
	} else
        {/* Unexpected request */
          Serial.print(F("Unexpected: "));
	  Serial.println(buf);
          wifly.flushRx();
	  Serial.println(F("Sending 404"));
	  send404();
	}

        wifly.flushRx();
            
        #ifdef DEBUG_MODE       
          Serial.println(F("Echo wifiavailable")); 
        #endif     

        
      }
    }
    
    /*
    while(Serial1.available()) {
      byte inByte = Serial1.read();
      Serial.write(inByte);
    }*/
  } 
  startTime = 0;
  stopTime = 0;

}

ISR(TIMER1_OVF_vect){
  //readSerials(); 

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

  if (!wifly.isAssociated()) 
  {
    if (wifly.join())
      wifiStatus = true;
    else
      wifiStatus = false;
  }
  
  if (tried == 4 && !wifiStatus) 
  {
          
    #ifdef DEBUG_MODE   
      Serial.print(F("Rebooting..."));
      
    #endif
    wifly.reboot();
    delay(3000);
    
    wifly.setSSID(ssid);
    wifly.setPassphrase(pass);
    wifly.enableDHCP();

    if (!wifly.join()) 
    {
    #ifdef DEBUG_MODE   
      Serial.println(F("Failed to join wifi network"));
    #endif
      terminal();
      //readSerials();
      wifiStatus = false;
    }else
      wifiStatus = true;
      
    tried = 0;
  }

  if (wifly.open(server, PORT) && wifiStatus) 
  {
    sensorsUpdate();
    #ifdef DEBUG_MODE     
      Serial.print(F(" Connected to "));
      Serial.println(server);
    #endif      

    //char itoaBuffer[8];
    byte i; 
   
    #ifdef DEBUG_MODE
      for(i=0; i<6; i++)
          Serial.println(HTTPPOST[i]);
       
       Serial.print(HTTPPOST[6]);
       ////itoa(strlen(bodyPOST),itoaBuffer,10);        
       ////wifly.println(itoaBuffer);
       Serial.println("273");
       Serial.println();
      
       for(i = 0; i<SENSORS_NUMBER; i++){ //11
        if(i==5) // flow meter
        {
          Serial.print(bodyJSON2[i]);
          Serial.print(getFlowMeter());
          
        }else{
          Serial.print(bodyJSON2[i]);
          Serial.print(value_sensors[i]);
        }
       }
     
       Serial.print(bodyJSON2[i]);
       Serial.print(bodyJSON2[i+1]);
       Serial.print(core.getRTC());
       Serial.println(bodyJSON2[i+2]);
    #endif

      for(i=0; i<6; i++)
          wifly.println(HTTPPOST[i]);
       
       wifly.print(HTTPPOST[6]);
       ////itoa(strlen(bodyPOST),itoaBuffer,10);        
       ////wifly.println(itoaBuffer);
       wifly.println("273");
       wifly.println();
        
       for(i = 0; i<SENSORS_NUMBER; i++){ //11
        if(i==5)
        {
          wifly.print(bodyJSON2[i]);
          wifly.print(getFlowMeter());
          
        }else{
          wifly.print(bodyJSON2[i]);
          wifly.print(value_sensors[i]);
        }
       }
     
       wifly.print(bodyJSON2[i]);
       wifly.print(bodyJSON2[i+1]);
       wifly.print(core.getRTC());
       wifly.print(bodyJSON2[i+2]);
       
    #ifdef DEBUG_MODE             
       Serial.print(F(" \nRegistro remoto con fecha: "));
       Serial.println(core.getRTC());
       //Serial.print(F(" \pulses "));
       //Serial.print(pulses);
       //Serial.print(F(" flowmeter  "));
       //Serial.print(getFlowMeter());
       
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
      
      
      if(!wifly.close());
      {
        #ifdef DEBUG_MODE       
          Serial.println(F("Failed to close tcp"));
        #endif
      }
      //connStatus = false;
      
  } else
  {
    wifiStatus = false;
    tried++;
    
    #ifdef DEBUG_MODE       
      Serial.print(F("Failed to open tcp tried# ")); Serial.println(tried);
    #endif
  }
  
  if(wifiStatus)
    readSerials();
    
  #ifdef DEBUG_MODE       
    Serial.println(F("Echo bucle")); 
  #endif     

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
  
  
}
/*
uint8_t Sensors::readDataDHT()
{
	// BUFFER TO RECEIVE
	uint8_t bits[5];
	uint8_t cnt = 7;
	uint8_t idx = 0;

	// EMPTY BUFFER
	for (byte i=0; i< 5; i++) bits[i] = 0;

	// REQUEST SAMPLE
	pinMode(DHT_PIN, OUTPUT);
	digitalWrite(DHT_PIN, LOW);
	delay(20);
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
	for (byte i=0; i<40; i++)
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
*/
boolean Sensors::readDHT() {
  uint8_t laststate = HIGH;
  uint8_t counter = 0;
  uint8_t j = 0, i;
  unsigned long currenttime;

  // Check if sensor was read less than two seconds ago and return early
  // to use last reading.
  currenttime = millis();
  if (currenttime < _lastreadtime) {
    // ie there was a rollover
    _lastreadtime = 0;
  }
  if (!firstreading && ((currenttime - _lastreadtime) < 2000)) {
    return true; // return last correct measurement
    //delay(2000 - (currenttime - _lastreadtime));
  }
  firstreading = false;

  _lastreadtime = millis();

  data[0] = data[1] = data[2] = data[3] = data[4] = 0;
  
  // pull the pin high and wait 250 milliseconds
  digitalWrite(DHT_PIN, HIGH);
  delay(250);

  // now pull it low for ~20 milliseconds
  pinMode(DHT_PIN, OUTPUT);
  digitalWrite(DHT_PIN, LOW);
  delay(20);
  noInterrupts();
  digitalWrite(DHT_PIN, HIGH);
  delayMicroseconds(40);
  pinMode(DHT_PIN, INPUT);

  // read in timings
  for ( i=0; i< MAXTIMINGS; i++) {
    counter = 0;
    while (digitalRead(DHT_PIN) == laststate) {
      counter++;
      delayMicroseconds(1);
      if (counter == 255) {
        break;
      }
    }
    laststate = digitalRead(DHT_PIN);

    if (counter == 255) break;

    // ignore first 3 transitions
    if ((i >= 4) && (i%2 == 0)) {
      // shove each bit into the storage bytes
      data[j/8] <<= 1;
      if (counter > _count)
        data[j/8] |= 1;
      j++;
    }

  }

  interrupts();
  
  // check we read 40 bits and that the checksum matches
  if ((j >= 40) && 
      (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) ) {
        
    humidity = (data[0]*256 + data[1])/10.0;   
    temperature = ((data[2] & 0x7F)*256 + data[3])/10.0;
    
    if (data[2] & 0x80)
	temperature *= -1;  
    return true;
  }
  
  return false;

}
/*
float Sensors::readHumidity() {
  float f;
  if(readDHT())
  {
      f = data[0];
      f *= 256;
      f += data[1];
      f /= 10;
      return f;
    }
  
  return NAN;
}

float Sensors::readTemperature() {
  float f;

  if (readDHT()) {
      f = data[2] & 0x7F;
      f *= 256;
      f += data[3];
      f /= 10;
      if (data[2] & 0x80)
	f *= -1;
      return f;
   }

  return NAN;
}
*/
float Sensors::getLDR(){
  float _valueLDR = avg(LDR_PIN);
  
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
  return avg(UV_PIN);
}

float Sensors::getNoise(){
  float _sound = avg(SOUND_PIN);
  
  if(_sound < 2)
    _sound = map(_sound, 0, 2, 1, 2);
  else 
    _sound = map(_sound, 3, 1023, 3, 1023);  
  
  _sound = 20*log10(_sound);
 
  return _sound;
}

float Sensors::getCO(){ 
  return avg(CO_PIN);
}

float  Sensors::getNO2(){ 
  float no2 = avg(NO2_PIN); // analogico A1
  //Serial.print("no2 "); Serial.println(no2);
  //no2 = (float)no2/1023*vcc; // voltage value from ADC 100K
  //no2 = ((Res0*vcc)/no2)-Res0;  // ohm value of the pull resistor
  //return no2;
  //return map(analogRead(NO2_PIN), 0, 1023, 0, 1000);
}

float Sensors::getFlowMeter(){
  float liters = pulses;
  liters /= 7.5;
  liters /= 60.0;
  return liters;
}

float Sensors::avg(byte pin) {
  byte times = 20;
  long acum = 0;
  
  for(byte i=0; i<times; i++){
    acum+= analogRead(pin);
    delay(2);
  }
  return(float)acum / times;  
}


void Sensors::sensorsUpdate(){

  //uint8_t check = readDataDHT();
  readDHT();
  //value_sensors[0] = temperature;
  //value_sensors[1] = humidity;   
  value_sensors[0] = temperature;//random(17,19);//readTemperature();
  value_sensors[1] = humidity;   
  
  value_sensors[2] = getLDR();
  value_sensors[3] = getUV();
  value_sensors[4] = getNoise();
  
  #ifdef ENABLED_INT_FLOWMETER 
    value_sensors[5] = getFlowMeter();
  #else
    value_sensors[5] = -1.0;
  #endif  

  value_sensors[6] = getNO2();
  value_sensors[7] = getCO(); 

#ifdef HAS_GPS
  //long latLgt[3];
  //latLgt[0] = 1.0;
  //latLgt[1] = 1.0;
  
  //getLatLgt(latLgt);
  value_sensors[8] = latLgt[0];
  value_sensors[9] = latLgt[1];
#else
  value_sensors[8] = 0.0;
  value_sensors[9] = 0.0;
#endif
  
}

