
/*
  Shift Register Example
 for 74HC595 shift register

 This sketch turns reads serial input and uses it to set the pins
 of a 74HC595 shift register.

 Hardware:
 * 74HC595 shift register attached to pins 2, 3, and 4 of the Arduino,
 as detailed below.
 * LEDs attached to each of the outputs of the shift register

 Created 22 May 2009
 Created 23 Mar 2010
 by Tom Igoe

 */
////Pin connected to Data in (DS) of 74HC595
const int dataPin = 8;
//Pin connected to latch pin (ST_CP) of 74HC595
const int latchPin = 7;
//Pin connected to clock pin (SH_CP) of 74HC595
const int clockPin = 6;


void setup() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
  // count from 0 to 255 and display the number 
  // on the LEDs
  for (byte numberToDisplay = 0; numberToDisplay < 15; numberToDisplay++) {
    // take the latchPin low so 
    // the LEDs don't change while you're sending in bits:
    digitalWrite(latchPin, LOW);
    // shift out the bits:
   
    
    shiftOut(dataPin, clockPin, MSBFIRST, numberToDisplay);  
    Serial.println(numberToDisplay);
    
    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
    // pause before next value:
    delay(1000);
  }
}
