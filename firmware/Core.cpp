#include <Wire.h>
//#include <SD.h>
#include <RTClib.h>

#include "Constants.h"
#include "Core.h"

RTC_DS1307 RTC; // object to RTC

//DateTime now;
//File logFile;

void Core::begin(){ //init variables
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
  // run clock with time PC
  if (! RTC.isrunning()) {
    //RTC.adjust(DateTime(__DATE__, __TIME__));
  }
}

/*void Resources::execute(){ // init program
 writeSD();
}
*/


// Struct format date end time (read)
String Core::getRTC(){

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

//printer data in SD card
void Core::writeSD(){
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

