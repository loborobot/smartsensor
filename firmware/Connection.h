#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include <Arduino.h>
#include <string.h>
//#include <WiFiRM04.h>

#define TX_SERIAL 2
#define RX_SERIAL 3

// class connection with WiFi/GSM
class Connection
{
public:
  boolean statusConn;
  boolean statusServer;
  //boolean modeAT;
  
  void begin();
  void printWifiStatus();
  void readSerials();
  
  void printData(String data);
  boolean httpPOST(const char* server, int port);
  void serverReceive();
  
  // RN-171
  boolean findInResponse(const char *toMatch,
                           unsigned int timeOut);
  boolean enterCommandMode();
  boolean attachWIFI();
  boolean connectTCP(const char *addr, int port);
  boolean disconnectTCP();
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


