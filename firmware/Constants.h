#ifndef __CONSTANST_H__
#define __CONSTANST_H__

#define DHT_PIN 9              

#define LDR_PIN A0 //pin LDR Analogico
#define NO2_PIN A1  //pin NO2 analogico   // dos resistencias en serie 51k , 820ohm
#define CO_PIN A2  //pin CO2 analogico  // dos resistencias en serie 100k , 820ohm
#define SOUND_PIN A4 //pin SOUND Analogico
#define UV_PIN A5 //pin UV Analogico

#define FLOWMETER_PIN 3 //pin CAUDAL Digital
#define MAXTIMINGS 85//40

// 13 -10 para softserial
// digital pins for interrruptions to ATMega32U4
//#define INT0 0 // Digital pin 3  
#define INT1 1 // Digital pin 2

#define ENABLED_INT_FLOWMETER


#define COMMAND_MODE_GUARD_TIME 250 // in milliseconds
#define DEBUG_MODE
//#define HAS_GPS
#define TYPE_LINK_CONNECTION 1 


//==============GPRS using GSM.h==========
/*
// PIN number if necessary
#define PINNUMBER ""

// APN information obrained from your network provider
#define GPRS_APN       "ba.amx" // replace with your GPRS APN
#define GPRS_LOGIN     "amx"    // replace with your GPRS login
#define GPRS_PASSWORD  "amx" // replace with your GPRS password

char server[] = "190.42.34.239"; // the base URL
char path[] = "/api/devices/"; // the path
int port = 8000; // the port, 80 for HTTP
*/

//============== GPRS end using GSM.h===========
/*
#define ONGPRS true
#define OFFGPRS false

// PIN number if necessary
#define PINNUMBER ""

// APN information obrained from your network provider
#define GPRS_APN       "ba.amx" // replace with your GPRS APN
#define GPRS_LOGIN     "amx"    // replace with your GPRS login
#define GPRS_PASSWORD  "amx" // replace with your GPRS password
*/

// ip server
//static char server[] = "192.168.1.201";
//static char server[] = "smartsensor.herokuapp.com";
static char server[] = "smartsensor-test.herokuapp.com";

#define PORT 80

//static  char ssid[] = "Lobo_Robot"; 
//static  char pass[] = "loborobot2015";   
#define POST_INTERVAL 10000

static  char ssid[] = "WLAN_A490"; 
static  char pass[] = "Z2C26C54CA490";   

// commans AT for set HLK-RM04 (not use because we use WiFiRM04 library)
/*static char *commands_wifi_client[13]={
  "at+netmode=2\r\n",
  "at+wifi_conf=WLAN_16D2,wpa2_aes,Z1460809D16D2\r\n",
  "at+dhcpc=1\r\n",
  "at+remoteip=192-168.1.200\r\n",
  "at+remoteport=8000\r\n",
  "at+remotepro=tcp\r\n",
  "at+timeout=0\r\n",
  "at+mode=server\r\n",
  "at+uart=115200,8,n,1\r\n",
  "at+uartpacklen=64\r\n",
  "at+uartpacktimeout=10\r\n",
  "at+net_commit=1\r\n",
  "at+reconn=1\r\n"
  };*/

static char* HTTPPOST[7]={
  "POST /webapi/update/ HTTP/1.1",
  "Host: smartsensor-test.herokuapp.com",
// "Host: 192.168.1.201",
  "User-Agent: SmartSensor",
  "Content-Type: application/json",
  "Accept: application/json",
  "Connection: close",
  "Content-Length: "
  };
    
static char* bodyJSON2[14]={
  "{\"tmp\":\"",
  "\",\"hum\":\"",
  "\",\"lig\":\"",
  "\",\"uv\":\"",
  "\",\"snd\":\"",
  "\",\"flm\":\"",
  "\",\"no2\":\"",
  "\",\"co\":\"",
  "\",\"id\":\"166d77ac1b46a1ec38aa35ab7e628ab5\"",
  ",\"tstamp\":\"",
  "\"}"
  };
  /*
static char* bodyJSON3[14]={
  "{\"temperature\": \"",
  "\",\"humidity\": \"",
  "\",\"light\": \"",
  "\",\"ultra_violet\": \"",
  "\",\"sound\": \"",
  "\",\"flowmeter\": \"",
  "\",\"nitrogen_dioxide\": \"",
  "\",\"carbon_monoxide\": \"",
  "\",\"latitude\": \"",
  "\",\"longitude\": \"",
  "\",\"device\": \"166d77ac1b46a1ec38aa35ab7e628ab5\"", 
  ",\"pub_date\": \"",
  "\"}"
  };*/

#endif
