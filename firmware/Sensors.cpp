#include "Constants.h"
#include "Sensors.h"

Sensors p_sens;

void rpm(){     //This is the function that the interupt calls 
  p_sens.NbTopsFan++;  //This function measures the rising and falling edge of the hall effect sensors signal
} 


void Sensors::begin(){ //init variables
  
  pinMode(FLOWMETER_PIN, INPUT); //initializes digital pin 2 as an input -flowmeter
  attachInterrupt(0,rpm,RISING); //and the interrupt is attached - flowmeter
  
  
}
void Sensors::execute(){ // init program
  
  uint8_t chk = readDataDHT11();//Temperature and humidity
  switch (chk)
  {
    case 0: 
            Serial.print("RH: ");
            Serial.print(((float)humidity11)/10, 2);Serial.print(" %");
            Serial.print(" Temperature: ");
            Serial.print(((float)temperature11)/10, 2);Serial.println(" (oC)");
            delay(2000);break;
    case -1: Serial.println("Checksum error"); break;
    case -2: Serial.println("Time out error"); break;
    default: Serial.println("Unknown error"); break;
  }
  
  float light = readDataLDR(); //LDR
  if(light == -2)
  {
    Serial.print("LDR (IN): ");
    Serial.print(getValueLDR());
    Serial.print (" LDR (OUT): Maxima ilumicacion");
  }
  else
  {  
    Serial.print("LDR (IN): ");
    Serial.print(getValueLDR());
    Serial.print (" LDR (OUT): ");
    Serial.print(light);  
    Serial.println(" lux");
  }
  
  float ultra_violet = readDataUV(); // UV
  Serial.print ("UV (IN): ");
  Serial.print(getValueUV());
  Serial.print (" UV (OUT): ");
  Serial.print(ultra_violet); 
  Serial.println(" UVI");
  
  Serial.print("Sound: "); //Sound
  Serial.println(readDataSound());
  
  readDataFlowmeter();
  
}

uint8_t Sensors::readDataDHT11() // temperature and humidity function
{
	// BUFFER TO RECEIVE
	uint8_t bits[5];
	uint8_t cnt = 7;
	uint8_t idx = 0;

	// EMPTY BUFFER
	for (int i=0; i< 5; i++) bits[i] = 0;

	// REQUEST SAMPLE
	pinMode(DHT11PIN, OUTPUT);
	digitalWrite(DHT11PIN, LOW);
	delay(18);
	digitalWrite(DHT11PIN, HIGH);
	delayMicroseconds(40);
	pinMode(DHT11PIN, INPUT);

	// ACKNOWLEDGE or TIMEOUT
	unsigned int loopCnt = 10000;
	while(digitalRead(DHT11PIN) == LOW)
		if (loopCnt-- == 0) return -2; //-2 : timeout

	loopCnt = 10000;
	while(digitalRead(DHT11PIN) == HIGH)
		if (loopCnt-- == 0) return -2; //-2 : timeout

	// READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT
	for (int i=0; i<40; i++)
	{
		loopCnt = 10000;
		while(digitalRead(DHT11PIN) == LOW)
			if (loopCnt-- == 0) return -2; //-2 : timeout

		unsigned long t = micros();

		loopCnt = 10000;
		while(digitalRead(DHT11PIN) == HIGH)
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
	humidity11    = bits[0] + bits[1]; 
	temperature11 = bits[2] + bits[3]; 

	uint8_t sum = bits[0] + bits[1] + bits[2] + bits[3];  

	if (bits[4] != sum) return -1; // -1 : checksum error
	return 0; //  0 : OK
}

float Sensors::readDataLDR() // LDR function
{
  float _minLDR = 5;
  float _maxLDR = 955;
  _valueLDR = analogRead(LDR_PIN); // read pin LDR
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

float Sensors::readDataUV() // UV function
{
  float _minUV = 0;
  float _maxUV = 30;
  _valueUV = analogRead(UV_PIN); // read pinUV
  _lastValueUV = _valueUV;
  _valueUV = constrain(_valueUV, _minUV, _maxUV); // value normalize
  _valueUV = map(_valueUV, _minUV, _maxUV, 0, 15); // Re-map
  return _valueUV;
}

float Sensors::readDataSound(){
 float sound = analogRead(SOUND_PIN); //Sound
 //valorSOUND = constrain(valorSOUND, 0, 100); // value normalize
 sound = map(sound, 0, 1023, 0, 100);
 return sound; 
}

void Sensors::readDataFlowmeter(){
 NbTopsFan = 0;   //Set NbTops to 0 ready for calculations
 //sei();      //Enables interrupts
 delay(1000);   //Wait 1 second
 //cli();      //Disable interrupts
 flowmeter = ((NbTopsFan * 60 / 5.5)/60); //(Pulse frequency x 60) / 5.5Q, = flow rate in L/hour 
 volume = ((flowmeter * 60)/1000);
 //Calc = (Calc / 60);
 //Serial.print (Calc, DEC); //Prints the number calculated above
 Serial.print((float)flowmeter, 2);
 Serial.print (" L/min\r\n"); //Prints "L/hour" and returns a  new line
 Serial.print((float)volume, 2);
 Serial.print (" m3/min\r\n"); //Prints "L/hour" and returns a  new line 
}
