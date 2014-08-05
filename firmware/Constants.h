//#define DHT22_PIN 3 //pin DHT22
#define DHT_PIN 8 //pin DHT11 Digital
#define LDR_PIN A0 //pin LDR Analogico
#define UV_PIN A5 //pin UV Analogico
#define SOUND_PIN 4 //pin SOUND Analogico
#define FLOWMETER_PIN 2 //pin CAUDAL Digital
//#define S900Rx_PIN 2 //pin RxSim900
//#define S900Tx_PIN 10 //pin TxSim900

























// FUNCTIONS FOR COMPILE ERRORS
void clearBuffer();
void appendToBuffer(char c);
void pushSlow(char* command,int charaterDelay=100,int endLineDelay=1000);
bool waitForString(char* string, long waitForMilliseconds=5000);
bool waitForString(char *string, int waitForMilliseconds=5000);
void waitForData(int wait_delay);
void bridge();
