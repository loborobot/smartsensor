//Arduino Code Lobo Robot Electronics
//Code by LoboRobotElectronics crew 2014
//Version 1.0

#include <Wire.h>
#include <RTClib.h>
#include <SD.h>
#include "Sensors.h"


Sensors psensors__;//objeto Sensores


//==============GPRS using GSM.h==========
/*
// PIN number if necessary
#define PINNUMBER ""

// APN information obrained from your network provider
#define GPRS_APN       "ba.amx" // replace with your GPRS APN
#define GPRS_LOGIN     "amx"    // replace with your GPRS login
#define GPRS_PASSWORD  "amx" // replace with your GPRS password

// initialize the library instances
GSMClient client;
GPRS gprs;
GSM gsmAccess;

// This example downloads the URL "http://arduino.cc/latest.txt"

char server[] = "190.42.34.239"; // the base URL
char path[] = "/api/devices/"; // the path
int port = 8000; // the port, 80 for HTTP
*/
//============== GPRS end using GSM.h===========

//byte valor; //eeprom

//File myFile;

void setup () {  
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  psensors__.begin();
}


void loop () {
  
  Serial.println("Loop");
  psensors__.execute();
  
  if (Serial1.available()) {
    Serial.write(Serial1.read());
    //if(Serial1.find("Connected")){
      //Serial.println("activado wifi");
    //}
  }
  
  // read from port 0, send to port 1:
  if (Serial.available()) {
    Serial1.write(Serial.read()); 
  }
}
