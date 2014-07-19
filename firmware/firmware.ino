//Arduino Code Lobo Robot Electronics
//Code by LoboRobotElectronics crew 2014
//Version 1.0

//#include <Wire.h>
//#include <RTClib.h>
#include <LoboRE.h>
#include <SoftwareSerial.h>
#include "utils.h"

#define DHT11PIN 7 //pin DHT11
#define DHT22_PIN 3 //pin DHT22
#define LDR_PIN 0 //pin LDR
#define UV_PIN 5 //pin UV
#define S900Rx_PIN 10 //pin RxSim900
#define S900Tx_PIN 11 //pin TxSim900

//========Star GPRS Sim900========================
Sim900 sim900gprs(S900Rx_PIN, S900Tx_PIN);
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
sDHT22 p_DHT22(DHT22_PIN); //object DHT22
//=======end DHT22============0

//=====start LDR=====
float valorLDR = 0.0;
sLDR p_LDR;
//====end LDR========== 

//=====start UV=======
float valorUV = 0.0;
sUV p_UV;
//========end UV ============


void setup () {  
  Serial.begin(9600);
  sim900gprs.setupSim900();
  //setupSim900();


  //Wire.begin();
  //startVal();
  //startRTC();
  //Serial.print("Read sensor: "); //DHT11
  //pinMode( ledPin, OUTPUT ); // ledPin how output   
}

void loop () {
  Serial.println(" ");
  //resultDisplay();
  startSim900();

  startSensorDHT11();
  //startSensorDHT22();
  startSensorLDR();
  startSensorUV();
  //displayRTC(); 
}



