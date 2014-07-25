//Arduino Code Lobo Robot Electronics
//Code by LoboRobotElectronics crew 2014
//Version 1.0

//#include <Wire.h>
//#include <RTClib.h>
#include <LoboRE.h>
//#include "utils.h"
#include <GSM.h>// import the GSM library


//#define DHT22_PIN 3 //pin DHT22
#define DHT11PIN 8 //pin DHT11 Digital
#define LDR_PIN A0 //pin LDR Analogico
#define UV_PIN A5 //pin UV Analogico
#define SOUND_PIN 4 //pin SOUND Analogico
#define FLOWMETER_PIN 6 //pin CAUDAL Digital
//#define S900Rx_PIN 2 //pin RxSim900
//#define S900Tx_PIN 10 //pin TxSim900

//========Star GPRS Sim900========================
//Sim900 sim900gprs(S900Rx_PIN, S900Tx_PIN);//sim900
//=========End GPRS Sim900=========================

//const int _led[]={3,5,6,9,10,11,13};//leds

//int _Value[5];//values analog write
//int _val[5];//values
//char _into[5];//values  
//int _dato1,_i=0;

//RTC_DS1307 RTC; //objeto RTC

//=========start DHT11======
sDHT11 p_dht11;//objeto DHT11
//=====end DHT1=============

//=======start DHT22==========
//sDHT22 p_DHT22(DHT22_PIN); //object DHT22
//=======end DHT22============0

//=====start LDR=====
float valorLDR = 0.0;
sLDR p_LDR;
//====end LDR========== 

//=====start UV=======
float valorUV = 0.0;
sUV p_UV;
//========end UV ============

//========start SOUND====
float valorSOUND =0.0;
//========end SOUND=======

//========start SOUND====

volatile int NbTopsFan; //measuring the rising edges of the signal
float Calc;                               
int hallsensor = 2;    //The pin location of the sensor
 
void rpm ()     //This is the function that the interupt calls 
{ 
  NbTopsFan++;  //This function measures the rising and falling edge of the hall effect sensors signal
} 
//========end SOUND=======

//==============GPRS using GSM.h==========

// PIN number if necessary
#define PINNUMBER ""

// APN information obrained from your network provider
#define GPRS_APN       "ba.amx" // replace with your GPRS APN
#define GPRS_LOGIN     "amx"    // replace with your GPRS login
#define GPRS_PASSWORD  "amx" // replace with your GPRS password

// initialize the library instances
GSMClient client;
GPRS gprs;
GSM gsmAccess;

// This example downloads the URL "http://arduino.cc/latest.txt"

char server[] = "190.42.34.239"; // the base URL
char path[] = "/api/devices/"; // the path
int port = 8000; // the port, 80 for HTTP


reqHTTP pHTTP;

float humidity = 20.43;
float temperature = 23.12;
float light = 3.02;
float ultra_violet = 15.2;
float sound = 40.56;
float flowmeter = 7.43;
float volume = 34.24;
float nitro_dioxide = 34.23;
float carbon_monoxide = 47.47;

//============== GPRS end using GSM.h===========

void setup () {  
  Serial.begin(9600);
  //sim900gprs.setupSim900();

  pinMode(hallsensor, INPUT); //initializes digital pin 2 as an input
  //Serial.begin(9600); //This is the setup function where the serial port is initialised,
  attachInterrupt(0, rpm, RISING); //and the interrupt is attached



  //Wire.begin();
  //startVal();
  //startRTC();
  //Serial.print("Read sensor: "); //DHT11
  //pinMode( ledPin, OUTPUT ); // ledPin how output   
  


//============GPRS USING GSM.h=========


  //String rGet = pHTTP.get();
  //Serial.println(rGet);
  //String jData = pHTTP.dataJson(humidity,temperature,light,ultra_violet,sound,flowmeter,volume,nitro_dioxide,carbon_monoxide);
  //Serial.println(jData);
  
  
  // initialize serial communications
  //Serial.begin(9600);
  
  Serial.println("Starting Arduino web client");
  // connection state
  boolean notConnected = true;

  // Start GSM shield
  // pass the PIN of your SIM as a parameter of gsmAccess.begin()
  while(notConnected)
  {
    if((gsmAccess.begin(PINNUMBER)==GSM_READY) &
        (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD)==GPRS_READY))
      notConnected = false;
    else
    {
      Serial.println("Not connected");
      delay(1000);
    }
  }

  Serial.println("connecting..");

  // if you get a connection, report back via serial:
  if (client.connect(server, port))
  {
    Serial.println("connected");
    // Make a HTTP request:
    client.println(pHTTP.get());
    
    
    //request POST
    /*client.println("POST /api/devices/ HTTP/1.0");
    client.println("Content-Type: application/json");
    client.println("Accept: application/json");
    client.println("Authorization: Basic ZmluY3l0OmZpbmN5dA==");
    client.println("Content-Length: 13");
    client.println();
    client.println("{\"gsm\": 3000}");
    client.println();*/
    
    
    //request
    /*client.println("GET /api/devices/ HTTP/1.0");
    client.println("Accept: application/json");
    client.println("Authorization: Basic ZmluY3l0OmZpbmN5dA==");
    client.println();*/
  } 
  else
  {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  
  //============END GPRS USING GSM.h=========

  
}



void loop () {
  //Serial.println(" ");
  //resultDisplay();
  //startSim900();

  
  //startSensorDHT22();
  /*startSensorDHT11();
  startSensorLDR();
  startSensorUV();
  startSensorSound();
  startFlowMeter();*/
  //displayRTC(); 
    
    
  
  //============GPRS USING GSM.h========= 
  /// if there are incoming bytes available 
  // from the server, read them and print them:
  
  if (client.available())
  {
    char c = client.read();
    Serial.print(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.available() && !client.connected())
  {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    // do nothing forevermore:
    for(;;)
      ;
  }
  
  //============END GPRS USING GSM.h=========
    
}
