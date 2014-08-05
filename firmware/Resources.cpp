#include <Wire.h>
#include <RTClib.h>
#include "Constants.h"
#include "Resources.h"

RTC_DS1307 RTC;

void Resources::begin(){ //init variables
  Wire.begin();
  RTC.begin();
  if (!RTC.isrunning()) {RTC.adjust(DateTime(__DATE__, __TIME__));}
}

void Resources::execute(){ // init program
 
}

void Resources::RTCread(){
  DateTime now = RTC.now();// funcion que regresa tiempo
 
    Serial.print(now.year(), DEC);//año
    Serial.print('/');
    Serial.print(now.month(), DEC);//mes
    Serial.print('/');
    Serial.print(now.day(), DEC);//dia
    Serial.print(' ');
    Serial.print(now.hour(), DEC);//hora
    Serial.print(':');
    Serial.print(now.minute(), DEC);//minuto
    Serial.print(':');
    Serial.print(now.second(), DEC);//segundo
    Serial.println();
    delay(950); 
}



