#include "Constants.h"
#include "Sensors.h"

Sensors p_sens;

void rpm(){     //This is the function that the interupt calls 
  p_sens.NbTopsFan++;  //This function measures the rising and falling edge of the hall effect sensors signal
} 


void Sensors::begin(){ //init variables
  
  pinMode(FLOWMETER_PIN, INPUT); //initializes digital pin 2 as an input -flowmeter
  attachInterrupt(0,rpm,RISING); //and the interrupt is attached - flowmeter
  
  
  // calibrate during the first five seconds 
  while (millis() < 5000) {
    _valueLDR = analogRead(LDR_PIN);
    // record the maximum sensor value
    if (_valueLDR > _valLDRmax) {_valLDRmax = _valueLDR;}
    // record the minimum sensor value
    if (_valueLDR < _valLDRmin) {_valLDRmin = _valueLDR;}
  }
  
  
}
void Sensors::execute(){ // init program
  updateSensor();  
}

void Sensors::updateSensor(){
  uint8_t chk = readDataDHT();// temperature and humidity
  uint8_t light = readDataLDR(); // LDR
  uint8_t ultra_violet = readDataUV(); // UV
  uint8_t sound = readDataSound(); // sound
  uint8_t flowvol= readDataFlowmeter(); // flowmeter and volume
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

uint8_t Sensors::readDataLDR() // LDR function
{
  _valueLDR = map(analogRead(LDR_PIN), _valLDRmin, _valLDRmax, 0, 255);
  _valueLDR = constrain(_valueLDR, 0, 255);
  return 0;
  
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

uint8_t Sensors::readDataUV(){ // UV function
  _valueUV = map(analogRead(UV_PIN), 0, 30, 0, 15); // read pinUV
  if (_valueUV >= 0.0) return 0;
  else return -1;
}

uint8_t Sensors::readDataSound(){
 _sound = map(analogRead(SOUND_PIN), 0, 1023, 0, 100); //Sound
 if (_sound >= 0.0) return 0;
 else return -1;
}

uint8_t Sensors::readDataFlowmeter(){
 NbTopsFan = 0;   //Set NbTops to 0 ready for calculations
 //sei();      //Enables interrupts
 delay(1000);   //Wait 1 second
 //cli();      //Disable interrupts
 _flowmeter = ((NbTopsFan * 60 / 5.5)/60); //(Pulse frequency x 60) / 5.5Q, = flow rate in L/hour 
 _volume = ((_flowmeter * 60)/1000); // m3/min
 if (_flowmeter >= 0.0 && _volume >= 0.0) return 0;
 else return -1;
}
