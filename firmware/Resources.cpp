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
  pinMode(10, OUTPUT);
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  //Serial.println("initialization done.");
  //logFile = SD.open("log.txt", FILE_WRITE);
  //logFile.close();

  pconn1__.begin();  
  Wire.begin();
  RTC.begin();
  if (RTC.isrunning()) {RTC.adjust(DateTime(__DATE__, __TIME__));}
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

void Resources::sendQuery(){
  boolean checkconn = pconn1__.verifyConnection();
  if(checkconn){
    for(int i=1; i<6; i++){
      Serial1.println(HTTPPOST[i]);
      if(i==4){
        Serial1.print(HTTPPOST[i]);
        Serial1.println(pconn1__.getMAC());
      }
      if(i==5){
        Serial1.print(HTTPPOST[i]);
        Serial1.println(strucData());
      }
    }
    Serial1.println(" ");
    sendData();
  }
}

void Resources::sendData(){
  boolean checkconn = pconn1__.verifyConnection();
  if(checkconn){
    Serial1.println(data);
    Serial1.flush();
  }
}

int Resources::strucData(){
  int lengthData, i = 0;

  data += JSONREQ[i];
  data += psens1__.convertF2C(psens1__._humidity);
  data += JSONREQ[i+1];
  data += psens1__.convertF2C(psens1__._valueLDR);
  data += JSONREQ[i+2];
  data += psens1__.convertF2C(psens1__._valueUV);
  data += JSONREQ[i+3];
  data += psens1__.convertF2C(psens1__._sound);
  data += JSONREQ[i+4];
  data += psens1__.convertF2C(psens1__._flowmeter);
  data += JSONREQ[i+5];
  data += psens1__.convertF2C(psens1__._volume);
  data += JSONREQ[i+6];
  data += psens1__.convertF2C(psens1__._CO2);
  data += JSONREQ[i+7];
  data += psens1__.convertF2C(psens1__._NO2);

  lengthData = data.length();
  return lengthData;
}
