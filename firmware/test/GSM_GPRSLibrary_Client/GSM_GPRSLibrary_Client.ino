#include "SIM900.h"
#include <SoftwareSerial.h>
#include "inetGSM.h"



InetGSM inet;

char msg[50];
int numdata;
char inSerial[50];
byte i=0;
boolean started=false;
boolean attached=false;
static char* server[5] = {
  "entel.pe",
  "Entel.PE",
  "Entel.pe",
  "modem.nextel.com.pe",
  "wap.nextel.com.pe"
};

// https://vsblogs.wordpress.com/2013/11/28/tcp-connection-over-gprs-using-sim900-and-at-commands 
// https://vsblogs.wordpress.com/2013/11/28/tcp-connection-over-gprs-using-sim900-and-at-commands/

char bodyPOST[]="{\"device\": \"166d77ac1b46a1ec38aa35ab7e628ab5\", \"pub_date\": \"2015-02-10T22:02:27.321Z\", \"temperature\": \"0\", \"humidity\": \"0\", \"light\": \"0\", \"ultra_violet\": \"0\", \"sound\": \"0\", \"flowmeter\": \"0\", \"volume\": \"0\", \"nitrogen_dioxide\": \"0\", \"carbon_monoxide\": \"0\", \"longitude\": \"1\", \"latitude\": \"1\"}";

void setup() 
{
  //Serial connection.
  Serial.begin(9600);
      
  Serial.println("GSM Shield testing.");
  //Start configuration of shield with baudrate.
  //For http uses is raccomanded to use 4800 or slower.
  if (gsm.begin(2400))
  {// 2400
    Serial.println(F("\nstatus=READY"));
    started=true;  
  }else
   Serial.println(F("\nstatus=IDLE"));
  delay(3000);
  
  while(started)
  {
      // WAP Entel
      // Entel.pe
       //if(inet.attachGPRS("wap.entel.pe","",""))
      //if (inet.attachGPRS("claro.pe", "claro", "claro"))
      //if (inet.attachGPRS("ba.amx", "amx", "amx"))

      if(i==5) break;
      if(attached) break;
      
      Serial.print(F("Trying to connect to "));
      Serial.println(server[i]);
      
      if(inet.attachGPRS(server[i],"","")){
        attached = true;
        Serial.print(F("Connected to "));
        Serial.println(server[i]);
      }else{
        Serial.print(F("Couln't connect to "));
        Serial.println(server[i]);
        attached = false;
      }    
      
      i++;
      delay(1000);
     
   
  }
   
  if(attached){  
    //Read IP address.
    gsm.SimpleWrite("AT+CIFSR"); 
    delay(5000);
    
    //Read until serial buffer is empty.
    gsm.WhileSimpleRead();
    numdata = inet.httpGET("www.google.com", 80, "/", msg, 50);
    delay(2000);
    Serial.println(F("\nNumber of data received:"));
    Serial.println(numdata);  
    Serial.println(F("\nData received:")); 
    Serial.println(msg); 
 
  }

}

byte post=1;

void loop() 
{        
    while(post < 4 && attached){
      numdata=inet.httpPOST("http://smartsensorv2.herokuapp.com", 80, "/webapi/update/", bodyPOST, msg, 50);
      
      
      Serial.println(F("\nNumber of data received post:"));
      Serial.println(numdata);  
      Serial.println(F("\nData received post:")); 
      Serial.println(msg);  
      

    
    delay(2000);
    post++;

    }
    //Read for new byte on serial hardware,
    //and write them on NewSoftSerial.
    serialhwread();
    //Read for new byte on NewSoftSerial.
    serialswread();

    //delay(2000);
}

void serialhwread(){
  i=0;
  if (Serial.available() > 0){            
    while (Serial.available() > 0) {
      inSerial[i]=(Serial.read());
      delay(10);
      i++;      
    }
    
    inSerial[i]='\0';
    if(!strcmp(inSerial,"/END")){
      Serial.println("_");
      inSerial[0]=0x1a;
      inSerial[1]='\0';
      gsm.SimpleWriteln(inSerial);
    }

    //Send a saved AT command using serial port.
    if(!strcmp(inSerial,"TEST")){
      Serial.println(F("SIGNAL QUALITY"));
      gsm.SimpleWriteln("AT+CSQ");
    }

    //Read last message saved.
    if(!strcmp(inSerial,"MSG")){
      Serial.println(msg);
    } else{
      Serial.println(inSerial);
      gsm.SimpleWriteln(inSerial);
    } 
    
    inSerial[0]='\0';
  }
}

void serialswread(){
  gsm.SimpleRead();
}
