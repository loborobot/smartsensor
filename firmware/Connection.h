#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include <Arduino.h>

#define TX_SERIAL 2
#define RX_SERIAL 3

#define COMMAND_MODE_GUARD_TIME 300 // in milliseconds
#define COMMAND_DELAY 5000

// class connection with WiFi/GSM
class Connection
{
public:
  boolean connStatus;
  boolean httpStatus;
  boolean wifiStatus;
  //boolean modeAT;
  
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
  boolean exitCommandMode();
  boolean attachWIFI();
  boolean connectTCP(const char *addr, int port);
  boolean disconnectTCP();
  void sendCommands(); 
  boolean reset();
  void repair();
   
  /* HLK-RM04
  void activeModeAT();
  void setConnection();
  boolean verifyConnection();
  boolean checkConnection();
  char* getMAC();*/
  
private:
};
#endif


