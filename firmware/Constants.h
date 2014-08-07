//#define DHT22_PIN 3 //pin DHT22
#define DHT_PIN 8 //pin DHT11 Digital
#define LDR_PIN A0 //pin LDR Analogico
#define UV_PIN A5 //pin UV Analogico
#define SOUND_PIN 4 //pin SOUND Analogico
#define FLOWMETER_PIN 2 //pin CAUDAL Digital
//#define S900Rx_PIN 2 //pin RxSim900
//#define S900Tx_PIN 10 //pin TxSim900



// PIN number if necessary
#define PINNUMBER ""

// APN information obrained from your network provider
#define GPRS_APN       "ba.amx" // replace with your GPRS APN
#define GPRS_LOGIN     "amx"    // replace with your GPRS login
#define GPRS_PASSWORD  "amx" // replace with your GPRS password


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
                  "{\"temperature\":\"",
                  "\",\"humidity\":\"", 
                  "\",\"light\":\"",
                  "\",\"ultra_violet\":\"",
                  "\",\"sound\":\"",
                  "\",\"flowmeter\":\"", 
                  "\",\"volume\":\"", 
                  "\",\"nitro_dioxide\":\"", 
                  "\",\"carbon_monoxide\":\"", 
                  "\"}"
                  };















// FUNCTIONS FOR COMPILE ERRORS
void clearBuffer();
void appendToBuffer(char c);
void pushSlow(char* command,int charaterDelay=100,int endLineDelay=1000);
bool waitForString(char* string, long waitForMilliseconds=5000);
bool waitForString(char *string, int waitForMilliseconds=5000);
void waitForData(int wait_delay);
void bridge();
