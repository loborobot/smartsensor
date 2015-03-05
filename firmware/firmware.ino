//Arduino Code
//Code by LoboRobotElectronics crew
//Version 1.0 - 2014

#include <Wire.h>
#include <RTClib.h>
#include <SoftwareSerial.h>

//#include <SD.h>
#include <WiFiRM04.h>
#include "Sensors.h"
#include "Connection.h"
#include "Constants.h"



Sensors sensors;//objeto Sensores

volatile int NbTopsFan; //measuring the rising edges of the signal (Flowmeter)
int Calc;  // (Flowmeter)
int vol; // (Flowmeter)

void rpm ()     //This is the function that the interupt calls (Flowmeter)
{ 
  NbTopsFan++;  //This function measures the rising and falling edge of the hall effect sensors signal (Flowmeter)
} 

void setup () {  
  
  Serial.begin(9600);//9600
  Serial1.begin(9600);//9600
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  } 
  
  sensors.begin(); // call init sensor
}

void loop () {
   sensors.execute();
}
