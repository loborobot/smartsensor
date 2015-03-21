#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include <Arduino.h>

#define TX_SERIAL 2
#define RX_SERIAL 3


#define SERIAL_TIMEOUT	50
#define COMMAND_MODE_GUARD_TIME 300 // in milliseconds
#define COMMAND_DELAY 5000

// define USE_ESCAPE_PIN to use HW pin to switch mode
#define USE_ESCAPE_PIN
#define ESCAPE_PIN  4  
#define ESCAPE_PIN_ACTIVE  LOW
// default Tes time should be 100 ms
#define TES_TIME_IN_MS	100
// defualt Tpt time should be 10ms
#define TPT_TIME_IN_MS	10
#define MODE_AT_ACTIVE  true


// class connection with WiFi/GSM

class Connection
{
public:
  boolean connStatus;
  boolean httpStatus;
  boolean wifiStatus;
  boolean modeAT;
  
  void begin();
  void printWifiStatus();
  void readSerials();
  
  boolean httpPOST(const char* server, int port, long *value_sensors);
  void serverReceive();
 
  // RN-171
  boolean waitResp(const char *toMatch, unsigned int timeOut);
  void simpleWrite(const __FlashStringHelper *cmd);
  void simpleWriteln(const __FlashStringHelper *cmd);
  
  void simpleWrite(const char *cmd);
  void simpleWriteln(const char *cmd);
  
  boolean enterCommandMode();
  boolean enterCommandModeHLK();
  boolean exitCommandMode();
  boolean attachWIFI();
  boolean connectTCP(const char *addr, int port);
  boolean disconnectTCP();
  void sendCommands(); 
  boolean reset();
  void repair();

  static void clearSerialRxData();
  static bool echoTest(long timeout = SERIAL_TIMEOUT);
  /* HLK-RM04
  void activeModeAT();
  void setConnection();
  boolean verifyConnection();
  boolean checkConnection();
  char* getMAC();*/
  
private:
};
#endif


