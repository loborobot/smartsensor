#include <Wire.h>
#include <SD.h>
#include <RTClib.h>
#include "Constants.h"
#include "Resources.h"
#include "Connection.h"
#include "Sensors.h"

Connection pconn1__;
Sensors psens1__;
RTC_DS1307 RTC;
File logFile;
DateTime now;

void Resources::begin(){ //init variables
  Serial.println("Resources Config");
  pinMode(10, OUTPUT);
  if (!SD.begin(4)) {
    Serial.println("SD Card initialization failed");
    //return;
  }
  //Serial.println("initialization done.");
  //logFile = SD.open("log.txt", FILE_WRITE);
  //logFile.close();
  //pconn1__.begin();  
  Wire.begin();
  RTC.begin();
  if (RTC.isrunning()) {RTC.adjust(DateTime(__DATE__, __TIME__));}
  Serial.println("END Resources Config");
}

/*void Resources::execute(){ // init program
 writeSD();
}
*/

void Resources::RTCread(){
  now = RTC.now();// funcion que regresa tiempo
 
}

void Resources::writeSD(){
  if(!pconn1__.statusConn){;
  logFile = SD.open("log.txt", FILE_WRITE);
    if(logFile){
      //Serial.print("Writing to test.txt...");
      logFile.print(now.year());logFile.print("/");logFile.print(now.month());logFile.print("/");logFile.print(now.day());logFile.print(" ");
      logFile.print(now.hour());logFile.print(":");logFile.print(now.minute());logFile.print(":");logFile.print(now.second());logFile.print(" ");
      logFile.print(psens1__._humidity);logFile.print(" ");logFile.print(psens1__._valueLDR);logFile.print(" ");logFile.print(psens1__._valueUV);
      logFile.print(" ");logFile.print(psens1__._sound);logFile.print(" ");logFile.print(psens1__._flowmeter);logFile.print(" ");
      logFile.print(psens1__._volume);logFile.print(" ");logFile.print(psens1__._CO2);logFile.print(" ");logFile.print(psens1__._NO2);
      }
      logFile.close();
    }
}



