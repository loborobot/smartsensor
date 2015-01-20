//#include <StringStream.h>

/*#include <GSM.h>
#include <EEPROMEx.h>
#include <Time.h>
#include <TimeAlarms.h>
//#include <StackList.h>
#include <Vector.h>

#define memoryBase_0 0 
#define memoryBase_1 1
#define memoryBase_2 10
#define memoryBase_3 20*/



//EEPROM externa
#include <EEPROMex.h>
#include <EEPROMvar.h>
//declarar

String remoteData;
char* uno2;

void setup(){
  
  Serial.begin(9600);
}
void loop(){
  if( Serial.available()){
    char c = Serial.read();
    //Serial.println(c);
    remoteData +=c;
    procesodata();
  }

}

int procesodata(){
    String uno = remoteData;
    String uno1; 
    String uno2;  
    String uno3;  
    String uno4;  
    String uno5;  
    String uno6;  
    String uno7;  
    String uno8;  


      
     remoteData.trim();
  
  uno1 = uno.substring(1,uno.indexOf(':'));
  uno  = uno.substring(uno.indexOf(':')+1,uno.length());
  uno2 = uno.substring(0,uno.indexOf(','));
  uno  = uno.substring(uno.indexOf(',')+1,uno.length());
  uno3 = uno.substring(0,uno.indexOf(':'));
  uno  = uno.substring(uno.indexOf(':')+1,uno.length());
  uno4 = uno.substring(0,uno.indexOf(','));
  uno  = uno.substring(uno.indexOf(',')+1,uno.length());
  uno5 = uno.substring(0,uno.indexOf(':'));
  uno  = uno.substring(uno.indexOf(':')+1,uno.length());
  uno6 = uno.substring(0,uno.indexOf(','));
  uno  = uno.substring(uno.indexOf(',')+1,uno.length());
  uno7 = uno.substring(0,uno.indexOf(':'));
  uno  = uno.substring(uno.indexOf(':')+1,uno.length());
  uno8 = uno.substring(0,uno.indexOf('}'));


     Serial.println(uno1);
     Serial.println(uno2);
     Serial.println(uno3);
     Serial.println(uno4);
     Serial.println(uno5);
     Serial.println(uno6);
     Serial.println(uno7);
     Serial.println(uno8);
         
}
//alarma
void createAlarm(byte id, const timeDayOfWeek_t dw, const int h_start, const int m_start, const int h_end, 
                    const int m_end, byte rele, boolean enabled){
    
    Alarm_t alr = {id, dw, h_start, m_start, h_end, m_end, rele, enabled};
    
    //startAlarm(dw, h_start, m_start, rele);
    //finishAlarm(dw, h_end, m_end, rele); 
                      
    EEPROM.writeBlock(id, alr);                       
}

void startAlarm(const timeDayOfWeek_t dw, const int h, const int m, int rele) {
   activate_rele = rele;
   Alarm.alarmRepeat(dw, h, m, 0, turnOnPort); 

   Serial.print("the alarm will turn on the day ");
   Serial.print(dw);
   Serial.print(" hour ");
   Serial.print(h);
   Serial.print(":");
   Serial.print(m);   
   Serial.print(" for port ");
   Serial.println(activate_rele);
   
  //Alarm.alarmRepeat(1, 10, 30, turnOffPort);
}

void finishAlarm(const timeDayOfWeek_t dw, const int h, const int m, byte rele){
  activate_rele = rele;
  Alarm.alarmRepeat(dw, h, m, 0, turnOffPort);
  Serial.print("the alarm will turn off the day ");
  Serial.print(dw);
  Serial.print(" hour ");
  Serial.print(h);
  Serial.print(":");
  Serial.print(m);   
  Serial.print(" for port ");
  Serial.println(activate_rele);

}
