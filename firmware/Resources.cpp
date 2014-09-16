#include <Wire.h>
//#include <SD.h>
#include <RTClib.h>
#include "Constants.h"
#include "Resources.h"
#include "Connection.h"
#include "Sensors.h"

Connection pconn1__;
Sensors psens1__;
RTC_DS1307 RTC;
//DateTime now;
//File logFile;

void Resources::begin(){ //init variables
  Serial.println("Resources Config");
  //pinMode(10, OUTPUT);
  //if (!SD.begin(4)) {
    //Serial.println("SD Card initialization failed");
    //return;
  //}
  //Serial.println("initialization done.");
  //logFile = SD.open("log.txt", FILE_WRITE);
  //logFile.close();
  //pconn1__.begin();  

  Wire.begin();
  RTC.begin();

  //if (! RTC.isrunning()) {
    //Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  //}
}

/*void Resources::execute(){ // init program
 writeSD();
}
*/

String Resources::RTCread(){
  String date;
  DateTime now = RTC.now();// funcion que regresa tiempo
  //  2014-07-15T22:02:27.321Z
  date += now.year();
  date += "-";
  date += now.month();
  date += "-";
  date += now.day();
  date += "T";
  date += now.hour();
  date += ":";
  date += now.minute();
  date += ":";
  date += now.second();
  date += ".000Z";
  
  return date;
}

void Resources::writeSD(){
  /*
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
    */
}



