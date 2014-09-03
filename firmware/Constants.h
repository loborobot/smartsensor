//#define DHT22_PIN 3 //pin DHT22
#define DHT_PIN 8 //pin DHT11 Digital
#define LDR_PIN A0 //pin LDR Analogico
#define UV_PIN A5 //pin UV Analogico
#define SOUND_PIN 4 //pin SOUND Analogico
#define FLOWMETER_PIN 2 //pin CAUDAL Digital
#define CO2_PIN A2  //pin CO2 analogico
#define NO2_PIN A1  //pin NO2 analogico

/*
#define SCAPE_PIN  21
#define SCAPE_PIN_ACTIVE  LOW
// default Tes time should be 100 ms
#define TES_TIME_IN_MS	100
// defualt Tpt time should be 10ms
#define TPT_TIME_IN_MS	10
#define MODE_AT_ACTIVE  true
*/

#define STATUS_OFFCONNECTION false
#define STATUS_ONCONNECTION true

#define TOKEN 166d77ac1b46a1ec38aa35ab7e628ab5

#define TOKEN1 6512bd43d9caa60276003fc202760200

//5eb63bbbe01eee0276003fc202760200
//5d41402abc4b2a0276003fc202760200

#define COMMAND_MODE_GUARD_TIME 250 // in milliseconds

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

static char* SERVER[1]={"192.168.1.200"};

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
  "at+reconn=1\r\n",
  };


static char* HTTPGET[4]={
  "GET /api/ HTTP/1.0",
  "Host: 192.168.1.200",
  "Accept: application/json",
  "Connection: close"
  };
  
  /*"190.42.34.239",
  "GET /api/devices/ HTTP/1.0 \n",
  "Accept: application/json \n",
  "Authorization: Basic "*/

static char* HTTPPOST[6]={
  "POST /api/update/ HTTP/1.0",
  "Host: 192.168.1.200",
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
                  

// Data JSON structure                  
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

/*
static char* bodyJSON[13]={
 "{",
 "\"device\":\"166d77ac1b46a1ec38aa35ab7e628ab5\",",
 "\"pub_date\":\"2014-07-15T22:02:27.321Z\",",
 "\"temperature\":\"0\",",
 "\"humidity\":\"0\",",
 "\"light\":\"0\",",
 "\"ultra_violet\":\"0\",",
 "\"sound\":\"0\",",
 "\"flowmeter\":\"0\",",
 "\"volume\":\"0\",",
 "\"nitrogen_dioxide\":\"0\",",
 "\"carbon_monoxide\":\"0\"",
 "}"
};
*/



