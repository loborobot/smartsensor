//#define DHT22_PIN 3 //pin DHT22
#define DHT_PIN 8 //pin DHT11 Digital
#define LDR_PIN A0 //pin LDR Analogico
#define UV_PIN A5 //pin UV Analogico
#define SOUND_PIN 4 //pin SOUND Analogico
#define FLOWMETER_PIN 2 //pin CAUDAL Digital

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
                  "190.42.34.239",
                  "GET //api/devices/ HTTP/1.0 \n",
                  "Accept: application/json \n",
                  "Authorization: Basic "  
                  };

static char* HTTPPOST[6]={
                  "190.42.34.239",
                  "POST /api/devices/ HTTP/1.0 \n", 
                  "Content-Type: application/json \n", 
                  "Accept: application/json \n", 
                  "Authorization: Basic ", 
                  "Content-Length: ", 
                  };

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

/*static char* DATAB[9]={
                  "Temperature: ",
                  "Humidity: ",
                  "Light: ",
                  "Ultra violet: ",
                  "Sound: ",
                  "Flowmeter: ",
                  "Volume: ",
                  "Carbon Monxide: ",
                  "Nitrogen Dioxide: ", 
                };*/













// FUNCTIONS FOR COMPILE ERRORS
void clearBuffer();
void appendToBuffer(char c);
void pushSlow(char* command,int charaterDelay=100,int endLineDelay=1000);
bool waitForString(char* string, long waitForMilliseconds=5000);
bool waitForString(char *string, int waitForMilliseconds=5000);
void waitForData(int wait_delay);
void bridge();
