#include <EEPROM.h>
#include <Wire.h> 
#include "RTClib.h"
#define disk1 0x50    //Address of 24LC256 eeprom chip

RTC_DS1307 RTC;

String remoteData;

    String IDT;  //ID TXT
    String IDN;  //ID #  
    String ENBT;  //Enable TXT  
    String ENBN;  //Enable #
    String TIST;  //Time start TXT
    String TISN;  //Time start #
    String TIET;  //Time end TXT
    String TIEN;  //Time end #
    String TIMT;  //Time TXT
    String TIMN;  //Time #
    String FRET;  //Frequency TXT
    String FREN;  //Frequency #

//    String ENBS = "TRUE";  // ENABLE comparation

    int FREN1=8;
    char c;

void setup(){
  Wire.begin(); // begin port I2C
  RTC.begin(); // begin comunication RTC
  RTC.adjust(DateTime(__DATE__, __TIME__)); // get date and hour  
  Serial.begin(9600);
}
void loop(){
  
/*  if( Serial.available()){
    c = Serial.read();
    Serial.println(c);
    remoteData +=c;ol
    procesodata();
  }*/
      DateTime now = RTC.now();
      Serial.print("week's day: ");
      Serial.println(now.dayOfWeek(), DEC); // L(1), M(2), X(3), J(4), V(5), S(6), D(7)
      delay(1000); // actualization each 1 seg.
}

int procesodata(){
  
  String uno = remoteData;
   
  remoteData.trim();
  
  IDT = uno.substring(1,uno.indexOf(':'));
  uno  = uno.substring(uno.indexOf(':')+1,uno.length());
  IDN = uno.substring(0,uno.indexOf(','));
  uno  = uno.substring(uno.indexOf(',')+1,uno.length());
  ENBT = uno.substring(0,uno.indexOf(':'));
  uno  = uno.substring(uno.indexOf(':')+1,uno.length());
  ENBN = uno.substring(0,uno.indexOf(','));
  uno  = uno.substring(uno.indexOf(',')+1,uno.length());
  TIST = uno.substring(0,uno.indexOf(':'));
  uno  = uno.substring(uno.indexOf(':')+1,uno.length());
  TISN = uno.substring(0,uno.indexOf(','));
  uno  = uno.substring(uno.indexOf(',')+1,uno.length());
  TIET = uno.substring(0,uno.indexOf(':'));
  uno  = uno.substring(uno.indexOf(':')+1,uno.length());
  TIEN = uno.substring(0,uno.indexOf(','));
  uno  = uno.substring(uno.indexOf(',')+1,uno.length());
  TIMT = uno.substring(0,uno.indexOf(':'));
  uno  = uno.substring(uno.indexOf(':')+1,uno.length());
  TIMN = uno.substring(0,uno.indexOf(','));
  uno  = uno.substring(uno.indexOf(',')+1,uno.length());
  FRET = uno.substring(0,uno.indexOf(':'));
  uno  = uno.substring(uno.indexOf(':')+1,uno.length());
  FREN = uno.substring(0,uno.indexOf('}'));


  Serial.println(IDT);
  Serial.println(IDN);
  Serial.println(ENBT);
  Serial.println(ENBN);
  Serial.println(TIST);
  Serial.println(TISN);
  Serial.println(TIET);
  Serial.println(TIEN);
  Serial.println(TIMT);
  Serial.println(TIMN);
  Serial.println(FRET);
  Serial.println(FREN);

  FREN1=FREN.toInt();
  
  if(FREN1 > 0 && FREN1 < 8){
    save_eeprom_ex();
  }

/*  if(ENBN == ENBS){
    save_eeprom_ex();
  }
*/         
}

void save_eeprom_ex(){
  

      Serial.println("<========================================== finish =================================================>");




}



/*
//************************ Eleccion de dia *******************
      DateTime now = RTC.now();
      Serial.print("week's day: ");
      Serial.println(now.dayOfWeek(), DEC); // L(1), M(2), X(3), J(4), V(5), S(6), D(7)
      delay(1000); // actualization each 1 seg.
//************************ fin *****************************        
*/
