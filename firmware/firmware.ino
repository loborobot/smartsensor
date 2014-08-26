//Arduino Code Lobo Robot Electronics
//Code by LoboRobotElectronics crew 2014
//Version 1.0

#include <Wire.h>
#include <RTClib.h>
#include <SD.h>
#include <WiFiRM04.h>
//#include <MD5.h>
#include "Sensors.h"
#include "Connection.h"

Sensors psensors__;//objeto Sensores

//==============GPRS using GSM.h==========
/*
// PIN number if necessary
#define PINNUMBER ""

// APN information obrained from your network provider
#define GPRS_APN       "ba.amx" // replace with your GPRS APN
#define GPRS_LOGIN     "amx"    // replace with your GPRS login
#define GPRS_PASSWORD  "amx" // replace with your GPRS password

char server[] = "190.42.34.239"; // the base URL
char path[] = "/api/devices/"; // the path
int port = 8000; // the port, 80 for HTTP
*/
//============== GPRS end using GSM.h===========

void setup () {  
  Serial.begin(115200);//9600
  Serial1.begin(115200);//9600
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  } 
/*
  //generate the MD5 hash for our string
  unsigned char* hash=MD5::make_hash("hello");
  //generate the digest (hex encoding) of our hash
  char *md5str = MD5::make_digest(hash, 16);
  //print it on our serial monitor
  //Serial.println(hash);
  Serial.println(md5str);
  */
  psensors__.begin();
}

void loop () {
  //pconn3__.readSerials();  
  psensors__.execute();
  //Serial.println("Loop");
}
