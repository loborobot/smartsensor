#include <Wire.h>
#include <SD.h>
#include <RTClib.h>
#include "Constants.h"
#include "Resources.h"
#include "Connection.h"
#include "Sensors.h"

Connection pconn__;
Sensors psens1__;
RTC_DS1307 RTC;
File logFile;
DateTime now;

void Resources::begin(){ //init variables
  pinMode(10, OUTPUT);
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  //logFile = SD.open("log.txt", FILE_WRITE);
  //logFile.close();

  pconn__.begin();  
  Wire.begin();
  RTC.begin();
  if (RTC.isrunning()) {RTC.adjust(DateTime(__DATE__, __TIME__));}
}

void Resources::execute(){ // init program
 writeSD();
}

void Resources::RTCread(){
  now = RTC.now();// funcion que regresa tiempo
 
}

void Resources::writeSD(){
  if(pconn__.statusConn == OFFGPRS){;
  logFile = SD.open("log.txt", FILE_WRITE);
    if(logFile){
      Serial.print("Writing to test.txt...");
      logFile.print(now.year());logFile.print("/");logFile.print(now.month());logFile.print("/");logFile.print(now.day());logFile.print(" ");
      logFile.print(now.hour());logFile.print(":");logFile.print(now.minute());logFile.print(":");logFile.print(now.second());logFile.print(" ");
      logFile.println(psens1__._humidity);
      }
      logFile.close();
    }
}




