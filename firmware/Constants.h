#ifndef __CONSTANST_H__
#define __CONSTANST_H__

#define STR1(z) #z
#define STR(z) STR1(z)
#define CONCAT(a,b,c) a STR(b) c

#define DHT_PIN 9              
#define LDR_PIN A0 //pin LDR Analogico
#define UV_PIN A5 //pin UV Analogico
#define SOUND_PIN A4 //pin SOUND Analogico
#define CO2_PIN A2  //pin CO2 analogico
#define NO2_PIN A1  //pin NO2 analogico
#define FLOWMETER_PIN 2 //pin CAUDAL Digital

// 13 -10 para softserial
// digital pins for interrruptions to ATMega32U4
#define INT0 0 // Digital pin 3  
#define INT1 1 // Digital pin 2
#define INT2 2 // Digital pin 0 
#define INT3 3 // Digital pin 1
#define INT4 4 // Digital pin 7

#define ENABLED_INT_FLOWMETER false

/*
#define SCAPE_PIN  21
#define SCAPE_PIN_ACTIVE  LOW
// default Tes time should be 100 ms
#define TES_TIME_IN_MS	100
// defualt Tpt time should be 10ms
#define TPT_TIME_IN_MS	10
#define MODE_AT_ACTIVE  true
*/

// check status conection
#define STATUS_OFFCONNECTION false
#define STATUS_ONCONNECTION true

#define COMMAND_MODE_GUARD_TIME 250 // in milliseconds
#define DEBUG_MODE true
#define HAS_GPS true

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
//static char* SERVER[1]={"192.168.1.200"};
static char SERVER[] = "http://remote-device.dyndns-at-work.com";
//String SERVER =  "remote-device.dyndns-at-work.com";
#define PORT 8080

// commans AT for set HLK-RM04 (not use because we use WiFiRM04 library)
static char *commands_wifi_client[13]={
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
  };

//query get for send serve
static char* HTTPGET[4]={
  "GET /api/ HTTP/1.0",
  "Host: remote-device.dyndns-ip.com",
  "Accept: application/json",
  "Connection: close"
  };
  
  /*"190.42.34.239",
  "GET /api/devices/ HTTP/1.0 \n",
  "Accept: application/json \n",
  "Authorization: Basic "*/

// query POST for send server
static char* HTTPPOST[6]={
  "POST /api/update/ HTTP/1.0",
  "Host: remote-device.dyndns-ip.com ",
  "Content-Type: application/json",
  "Accept: application/json",
  "Content-Length: ",
  "Connection: close\n"
  };
                
  /*"190.42.34.239",
  "POST /api/devices/ HTTP/1.0 \n", 
  "Content-Type: application/json \n", 
  "Accept: application/json \n", 
  //"Authorization: Basic ", 
  "Content-Length: ", */
                  
   
// 1: data JSON structue               
static char* bodyJSON[14]={
  "{\"device\": \"166d77ac1b46a1ec38aa35ab7e628ab5\",",
  "\"pub_date\": \"",
  "\",\"temperature\": ",
  ",\"humidity\": ",
  ",\"light\": ",
  ",\"ultra_violet\": ",
  ",\"sound\": ",
  ",\"flowmeter\": ",
  ",\"volume\": ",
  ",\"nitrogen_dioxide\": ",
  ",\"carbon_monoxide\": ",
  ",\"latitude\": ",
  ",\"longitude\": ",
  "}"
  };


// 2: Data JSON structure                  
static char* JSONREQ[10]={
  "{\"temperature\": \"",
  "\",\"humidity\": \"", 
  "\",\"light\": \"",
  "\",\"ultra_violet\": \"",
  "\",\"sound\": \"",
  "\",\"flowmeter\": \"", 
  "\",\"volume\": \"", 
  "\",\"nitro_dioxide\": \"", 
  "\",\"carbon_monoxide\": \"", 
  "\"}"
  };
  
// 3: data JSON structue               
static char* bodyJSON2[14]={
  "{\"temp\": ",
  ",\"hum\": ",
  ",\"light\": ",
  ",\"uv\": ",
  ",\"sound\": ",
  ",\"flowmeter\": ",
  ",\"volume\": ",
  ",\"no2\": ",
  ",\"co\": ",
  ",\"latitude\": ",
  ",\"longitude\": ",
  ",\"timestamp\": \"",
  "\",\"device\": \"166d77ac1b46a1ec38aa35ab7e628ab5\"",
  "}"
  };

#endif
