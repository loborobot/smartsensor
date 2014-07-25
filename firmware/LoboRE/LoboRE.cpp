/*Lobo Robot Electronics*/

#include <Arduino.h>
#include "LoboRE.h"

#include "pins_arduino.h" //DHT22

//#if ARDUINO >= 100 //DHT22
#include "Arduino.h" //DHT22
#include <avr/io.h> //DHT22
#include <avr/interrupt.h> //DHT22
#include <avr/pgmspace.h> //DHT22
#include <string.h> //Sim900
#include <stdio.h>
//#include <../SoftwareSerial/SoftwareSerial.h> //Sim900

#define DIRECT_READ(base, mask)		(((*(base)) & (mask)) ? 1 : 0) //DHT22
#define DIRECT_MODE_INPUT(base, mask)	((*(base+1)) &= ~(mask)) //DHT22
#define DIRECT_MODE_OUTPUT(base, mask)	((*(base+1)) |= (mask)) //DHT22
#define DIRECT_WRITE_LOW(base, mask)	((*(base+2)) &= ~(mask)) //DHT22
//#define DIRECT_WRITE_HIGH(base, mask)	((*(base+2)) |= (mask)) //DHT22

// This should be 40, but the sensor is adding an extra bit at the start
#define DHT22_DATA_BIT_COUNT 41 //DHT22


//=========================START DHT11===================
// returnvalues:
//  0 : OK
// -1 : checksum error
// -2 : timeout
int sDHT11::read(int pin)
{
	// BUFFER TO RECEIVE
	uint8_t bits[5];
	uint8_t cnt = 7;
	uint8_t idx = 0;

	// EMPTY BUFFER
	for (int i=0; i< 5; i++) bits[i] = 0;

	// REQUEST SAMPLE
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
	delay(18);
	digitalWrite(pin, HIGH);
	delayMicroseconds(40);
	pinMode(pin, INPUT);

	// ACKNOWLEDGE or TIMEOUT
	unsigned int loopCnt = 10000;
	while(digitalRead(pin) == LOW)
		if (loopCnt-- == 0) return -2;

	loopCnt = 10000;
	while(digitalRead(pin) == HIGH)
		if (loopCnt-- == 0) return -2;

	// READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT
	for (int i=0; i<40; i++)
	{
		loopCnt = 10000;
		while(digitalRead(pin) == LOW)
			if (loopCnt-- == 0) return -2;

		unsigned long t = micros();

		loopCnt = 10000;
		while(digitalRead(pin) == HIGH)
			if (loopCnt-- == 0) return -2;

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
	humidity    = bits[0] + bits[1]; 
	temperature = bits[2] + bits[3]; 

	uint8_t sum = bits[0] + bits[1] + bits[2] + bits[3];  

	if (bits[4] != sum) return -1;
	return 0;
}
//===============END DHT11=========================

//=============START DHT22=========================
sDHT22::sDHT22(uint8_t pin)
{
    _bitmask =  digitalPinToBitMask(pin);
    _baseReg = portInputRegister(digitalPinToPort(pin));
    _lastReadTime = millis();
    _lastHumidity = DHT22_ERROR_VALUE;
    _lastTemperature = DHT22_ERROR_VALUE;
}

//
// Read the 40 bit data stream from the DHT 22
// Store the results in private member data to be read by public member functions
//
DHT22_ERROR_t sDHT22::readData()
{
  uint8_t bitmask = _bitmask;
  volatile uint8_t *reg asm("r30") = _baseReg;
  uint8_t retryCount;
  uint8_t bitTimes[DHT22_DATA_BIT_COUNT];
  int currentHumidity;
  int currentTemperature;
  uint8_t checkSum, csPart1, csPart2, csPart3, csPart4;
  unsigned long currentTime;
  int i;

  currentHumidity = 0;
  currentTemperature = 0;
  checkSum = 0;
  currentTime = millis();
  for(i = 0; i < DHT22_DATA_BIT_COUNT; i++)
  {
    bitTimes[i] = 0;
  }

  if(currentTime - _lastReadTime < 2000)
  {
    // Caller needs to wait 2 seconds between each call to readData
    return DHT_ERROR_TOOQUICK;
  }
  _lastReadTime = currentTime;

  // Pin needs to start HIGH, wait until it is HIGH with a timeout
  cli();
  DIRECT_MODE_INPUT(reg, bitmask);
  sei();
  retryCount = 0;
  do
  {
    if (retryCount > 125)
    {
      return DHT_BUS_HUNG;
    }
    retryCount++;
    delayMicroseconds(2);
  } while(!DIRECT_READ(reg, bitmask));
  // Send the activate pulse
  cli();
  DIRECT_WRITE_LOW(reg, bitmask);
  DIRECT_MODE_OUTPUT(reg, bitmask); // Output Low
  sei();
  delayMicroseconds(1100); // 1.1 ms
  cli();
  DIRECT_MODE_INPUT(reg, bitmask);	// Switch back to input so pin can float
  sei();
  // Find the start of the ACK Pulse
  retryCount = 0;
  do
  {
    if (retryCount > 25) //(Spec is 20 to 40 us, 25*2 == 50 us)
    {
      return DHT_ERROR_NOT_PRESENT;
    }
    retryCount++;
    delayMicroseconds(2);
  } while(!DIRECT_READ(reg, bitmask));
  // Find the end of the ACK Pulse
  retryCount = 0;
  do
  {
    if (retryCount > 50) //(Spec is 80 us, 50*2 == 100 us)
    {
      return DHT_ERROR_ACK_TOO_LONG;
    }
    retryCount++;
    delayMicroseconds(2);
  } while(DIRECT_READ(reg, bitmask));
  // Read the 40 bit data stream
  for(i = 0; i < DHT22_DATA_BIT_COUNT; i++)
  {
    // Find the start of the sync pulse
    retryCount = 0;
    do
    {
      if (retryCount > 35) //(Spec is 50 us, 35*2 == 70 us)
      {
        return DHT_ERROR_SYNC_TIMEOUT;
      }
      retryCount++;
      delayMicroseconds(2);
    } while(!DIRECT_READ(reg, bitmask));
    // Measure the width of the data pulse
    retryCount = 0;
    do
    {
      if (retryCount > 50) //(Spec is 80 us, 50*2 == 100 us)
      {
        return DHT_ERROR_DATA_TIMEOUT;
      }
      retryCount++;
      delayMicroseconds(2);
    } while(DIRECT_READ(reg, bitmask));
    bitTimes[i] = retryCount;
  }
  // Now bitTimes have the number of retries (us *2)
  // that were needed to find the end of each data bit
  // Spec: 0 is 26 to 28 us
  // Spec: 1 is 70 us
  // bitTimes[x] <= 11 is a 0
  // bitTimes[x] >  11 is a 1
  // Note: the bits are offset by one from the data sheet, not sure why
  for(i = 0; i < 16; i++)
  {
    if(bitTimes[i + 1] > 11)
    {
      currentHumidity |= (1 << (15 - i));
    }
  }
  for(i = 0; i < 16; i++)
  {
    if(bitTimes[i + 17] > 11)
    {
      currentTemperature |= (1 << (15 - i));
    }
  }
  for(i = 0; i < 8; i++)
  {
    if(bitTimes[i + 33] > 11)
    {
      checkSum |= (1 << (7 - i));
    }
  }

  _lastHumidity = (float(currentHumidity & 0x7FFF) / 10.0);
  if(currentTemperature & 0x8000)
  {
   // Below zero, non standard way of encoding negative numbers!
    currentTemperature &= 0x7FFF;
    _lastTemperature = (float(currentTemperature) / 10.0) * -1.0;
  }
  else
  {
    _lastTemperature = float(currentTemperature) / 10.0;
  }

  csPart1 = currentHumidity >> 8;
  csPart2 = currentHumidity & 0xFF;
  csPart3 = currentTemperature >> 8;
  csPart4 = currentTemperature & 0xFF;
  if(checkSum == ((csPart1 + csPart2 + csPart3 + csPart4) & 0xFF))
  {
    return DHT_ERROR_NONE;
  }
  return DHT_ERROR_CHECKSUM;
}
//================END DHT22==================

//====================START LDR==============
float sLDR::readDataLDR(uint8_t pinLDR)
{
  _pinLDR = pinLDR;
  _minLDR = 5;
  _maxLDR = 955;
  _valueLDR = analogRead(_pinLDR); // read pin LDR
  _lastValueLDR = _valueLDR;
  _valueLDR = constrain(_valueLDR, _minLDR, _maxLDR); // value normalize
  
  if(_valueLDR < 5) return 0.0;
  if(_valueLDR>=5 && _valueLDR <= 36){
    _valueLDR = map(_valueLDR, 5, 36, 1, 10);
    return (_valueLDR/10);
  }
  if(_valueLDR>36 && _valueLDR <= 215){
    _valueLDR = map(_valueLDR, 36, 215, 1, 10);
    return _valueLDR; 
  }
  if(_valueLDR>215 && _valueLDR <= 477){
    _valueLDR = map(_valueLDR, 215, 477, 10, 40);
    return _valueLDR;
  }
  if(_valueLDR>477 && _valueLDR <= 673){
    _valueLDR = map(_valueLDR, 477, 673, 40, 100);
    return _valueLDR;
  }
  if (_valueLDR>673 && _valueLDR <= 795){
    _valueLDR = map(_valueLDR, 673, 795, 100, 200);
    return _valueLDR;
  }
  if(_valueLDR>795 && _valueLDR <= 955){
    _valueLDR = map(_valueLDR, 795, 955, 200, 1000);
    return _valueLDR;
  }
  if(_valueLDR>795 && _valueLDR <= 955){
    _valueLDR = map(_valueLDR, 795, 955, 200, 1000);
    return _valueLDR;
  }
  if(_valueLDR>1000) return -2;
}
//=================END LDR==================

//====================START LDR==============
float sUV::readDataUV(uint8_t pinUV)
{
  _pinUV = pinUV;
  _minUV = 0;
  _maxUV = 30;
  _valueUV = analogRead(_pinUV); // read pinUV
  _lastValueUV = _valueUV;
  _valueUV = constrain(_valueUV, _minUV, _maxUV); // value normalize
  _valueUV = map(_valueUV, _minUV, _maxUV, 0, 15); // Re-map
  return _valueUV;
}
//=================END LDR==================

//=====================START SIM900======================


Sim900::Sim900(int rx, int tx) : mySerial(rx,tx){

 buffSize = 0;
 lastInt = 0;
 sendMessage = false;
 startShutDown = false;

 currentStatusGSM = GSMOFFLINE;
 currentStatusGPRS = GPRSDISCONNECTED;
}

void Sim900::setupSim900(){
  // init myserial
  mySerial.begin(9600);
  if(!sendMessage){
    sendMessage = true;
    startShutDown = true;
  }
  Serial.print("Arduino: Setup finished.\r\n");
}


void Sim900::shutDown() {
  pushSlow("\r\n");
  pushSlow("AT+CPOWD=1\r\n");
}

void Sim900::shutdownGRPS() {
  pushSlow("AT+CIPSHUT\r\n");
  waitForData(3000);
  pushSlow("AT+CGATT=0\r\n");
  waitForData(3000);
  currentStatusGPRS = 0;
}

void Sim900::setupGPRS() {
  pushSlow("\r\n");
  waitForData(1000);
  shutdownGRPS();
  pushSlow("AT+CGATT=1\r\n");
  if(!waitForString("OK",10000)) {
    shutdownGRPS();
    return;
  }
  pushSlow("AT+CSTT=\"claro.pe\",\"claro\",\"claro\"\r\n");
  if(!waitForString("OK",10000)) {
    shutdownGRPS();
    return;
  }
  pushSlow("AT+CIICR\r\n"); 
  if(!waitForString("OK",10000)) {
    shutdownGRPS();
    return;
  }
  pushSlow("AT+CIFSR\r\n"); 
  waitForData(1000);
  currentStatusGPRS = 1;
}

void Sim900::GET() {
  pushSlow("\r\n");
  waitForData(1000);
  pushSlow("AT+CIPSTART=\"TCP\",\"192.168.1.200\",8000\r\n");
  if(!waitForString("CONNECT OK",30000)) {
    return;
  }
  pushSlow("AT+CIPSEND\r\n");
  waitForData(1000);
  pushSlow("GET /api/ HTTP/1.1\r\n");
  pushSlow("Host: 192.168.1.35\r\n");
  pushSlow("Connection: Keep-Alive\r\n");
  pushSlow("\r\n");
  pushSlow("\x1A");
  if(!waitForString("SEND OK",15000)) {
    return;
  }
  waitForData(15000);
  pushSlow("AT+CIPCLOSE\r\n");
  if(!waitForString("CLOSE OK",5000)) {
    return;
  }
}


boolean Sim900::checkForSIM900() {
  // Figure out if already connected
  pushSlow("AT\r\n");
  if(!waitForString("OK",5000))
    return false;
  pushSlow("AT+CPIN?\r\n");
  if(!waitForString("+CPIN: READY",5000))
    return false;
  pushSlow("AT+CFUN?\r\n");
  if(!waitForString("+CFUN: 1",5000))
    return false;
  pushSlow("AT+CCALR?\r\n");
  if(!waitForString("+CCALR: 1",5000))
    return false;
  return true;
}


void Sim900::clearBuffer() {
  for(int i=0;i<BUFFSIZE;i++) {
    buffer[i] = 0;
  }
  buffSize = 0;
}

void Sim900::appendToBuffer(char c) {
  if(buffSize<BUFFSIZE) {
    buffer[buffSize++] = c;
  } else {
    Serial.print(buffer);
    clearBuffer();
    buffer[buffSize++] = c;
  }
}

void Sim900::pushSlow(char* command,int charaterDelay,int endLineDelay) {
  for(int i=0; i<strlen(command); i++) {
    mySerial.write(command[i]);
    if(command[i]=='\n') {
      delay(endLineDelay);
    } else {
      delay(charaterDelay);
    }
  }
}

bool Sim900::waitForString(char* string, long waitForMilliseconds) {
  clearBuffer();
  long startTime = millis();
  boolean found = false;
  Serial.print("Waiting for string: \"");
  Serial.print(string);
  Serial.print("\"\r\n");
  while((millis()-startTime)<waitForMilliseconds) {
    if (mySerial.available()) {
      chr = mySerial.read();
      Serial.write((char)chr);
      appendToBuffer((char)chr);
      if((char)chr=='\n') {
        if(strstr(buffer,string)!=NULL) {
          found = true;
          break;
        } else if(strstr(buffer,"ERROR")!=NULL) {
          found = false;
          break;
        }
        clearBuffer();
      }
    }
    if (Serial.available())
      mySerial.write(Serial.read());
  }
  return found;
}


void Sim900::waitForData(int wait_delay) {
  long startTime = millis();
  while((millis()-startTime)<wait_delay) {
    if (mySerial.available())
      Serial.write(mySerial.read());
    if (Serial.available()) {
      mySerial.write(Serial.read());
    }
  }
}

void Sim900::bridge() {
  if (mySerial.available())
    Serial.write(mySerial.read());
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}

//==========================END SIM900========================

//==================START HTTP====================
String reqHTTP::get(){
  reqGet = "GET /api/devices/ HTTP/1.0\nAccept: application/json\nAuthorization: Basic ZmluY3l0OmZpbmN5dA==\n";
  return reqGet;

}

String reqHTTP::post(){
  reqPost = "POST /api/devices/ HTTP/1.0\nContent-Type: application/json\nAccept: application/json\nAuthorization: Basic ZmluY3l0OmZpbmN5dA==\nContent-Length: 13";
  return reqPost;

}

String reqHTTP::dataJson(float humidity,float temperature, float light, float ultra_violet, float sound, float flowmeter, float volume, float nitro_dioxide, float carbon_monoxide){
  
  data = "{\"gsm\": 1";
  data += "\n\"temperature\": ";
  data += convertF2C(temperature);
  data += "\n\"humidity\": ";
  data += convertF2C(humidity);
  data += "\n\"light\": ";
  data += convertF2C(light);
  data += "\n\"ultra_violet\": ";
  data += convertF2C(ultra_violet);
  data += "\n\"sound\": ";
  data += convertF2C(sound);
  data += "\n\"flowmeter\": ";
  data += convertF2C(flowmeter);
  data += "\n\"volume\": ";
  data += convertF2C(volume);
  data += "\n\"nitro_dioxide\": ";
  data += convertF2C(nitro_dioxide);
  data += "\n\"carbon_monoxide\": ";
  data += convertF2C(carbon_monoxide);
  data += "}";
  
  return data;
  //Serial.println(data);
}

char* reqHTTP::convertF2C(float val){
  static char dtostrfbuffer[10];
  return dtostrf(val,5, 2, dtostrfbuffer);
}
//==================END HTTP======================

//=================START GPRS=============
