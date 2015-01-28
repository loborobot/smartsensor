//Arduino Code
//Code by LoboRobotElectronics crew
//Version 1.0 - 2014

#include <Wire.h>
#include <RTClib.h>
//#include <SD.h>
#include <WiFiRM04.h>
#include "Sensors.h"
#include "Connection.h"
#include "Constants.h"

Sensors psensors__;//objeto Sensores

volatile int NbTopsFan; //measuring the rising edges of the signal (Flowmeter)
int Calc;  // (Flowmeter)
int vol; // (Flowmeter)

void rpm ()     //This is the function that the interupt calls (Flowmeter)
{ 
  NbTopsFan++;  //This function measures the rising and falling edge of the hall effect sensors signal (Flowmeter)
} 

void setup () {  
  
  pinMode(FLOWMETER_PIN, INPUT); //initializes digital pin 2 as an input (Flowmeter)
  
  Serial.begin(115200);//9600
  Serial1.begin(115200);//9600
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  } 
  
  attachInterrupt(0, rpm, RISING); //and the interrupt is attached (Flowmeter)
  
  psensors__.begin(); // call init sensor
}

void loop () {
  // (Flowmeter) - start
  NbTopsFan = 0;   //Set NbTops to 0 ready for calculations
  //sei();      //Enables interrupts
  delay (1000);   //Wait 1 second
  //cli();      //Disable interrupts
  Calc = ((NbTopsFan * 60 / 5.5)/60); //(Pulse frequency x 60) / 5.5Q, = flow rate in L/hour 
  vol = (Calc * 1); // m3/min
  // (Flowmeter)- end
  
  
  psensors__.execute(Calc,vol);
  //delay(2*60*1000);
}