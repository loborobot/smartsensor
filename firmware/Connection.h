#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include <Arduino.h>
#include <string.h>
#include <WiFiRM04.h>

class Connection
{
public:
  boolean statusConn;
  boolean statusServer;
  //boolean modeAT;
  
  void begin();
  void printWifiStatus();
  void readSerials();
  boolean findInResponse(const char *toMatch,
                           unsigned int timeOut);
  boolean enterCommandMode();
  boolean connect();
  boolean open(const char *addr, int port);
  boolean close();
  boolean reset();
  void repair();
  
  
  void printData(String data);
  boolean sendQueryData(String data);

  /*
  void activeModeAT();
  void setConnection();
  boolean verifyConnection();
  boolean checkConnection();
  char* getMAC();*/
  
private:
};
#endif


