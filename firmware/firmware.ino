//Arduino Code Lobo Robot Electronics
//Code by LoboRobotElectronics crew 2014
//Version 1.0

//#include <Wire.h>
//#include <RTClib.h>
#include <LoboRE.h>
#include <SoftwareSerial.h>
#include "utils.h"

#define DHT11PIN 7 //pin DHT11
//#define DHT22_PIN 3 //pin DHT22
#define LDR_PIN 0 //pin LDR
#define UV_PIN 5 //pin UV
#define SOUND_PIN A4 //pin SOUND
//#define FLOWMETER_PIN 6 //pin CAUDAL
#define S900Rx_PIN 10 //pin RxSim900
#define S900Tx_PIN 11 //pin TxSim900

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
// reading liquid flow rate using Seeeduino and Water Flow Sensor from Seeedstudio.com
// Code adapted by Charles Gantt from PC Fan RPM code written by Crenn @thebestcasescenario.com
// http:/themakersworkbench.com http://thebestcasescenario.com http://seeedstudio.com
 
volatile int NbTopsFan; //measuring the rising edges of the signal
float Calc;                               
int hallsensor = 2;    //The pin location of the sensor
 
void rpm ()     //This is the function that the interupt calls 
{ 
  NbTopsFan++;  //This function measures the rising and falling edge of the hall effect sensors signal
} 
//========end SOUND=======

void setup () {  
  //Serial.begin(9600);
  //sim900gprs.setupSim900();

  pinMode(hallsensor, INPUT); //initializes digital pin 2 as an input
  Serial.begin(9600); //This is the setup function where the serial port is initialised,
  attachInterrupt(0, rpm, RISING); //and the interrupt is attached

  //Wire.begin();
  //startVal();
  //startRTC();
  //Serial.print("Read sensor: "); //DHT11
  //pinMode( ledPin, OUTPUT ); // ledPin how output   
}

void loop () {
  Serial.println(" ");
  //resultDisplay();
  //startSim900();

  startSensorDHT11();
  //startSensorDHT22();
  startSensorLDR();
  startSensorUV();
  startSensorSound();
  startFlowMeter();
  //displayRTC(); 
}



