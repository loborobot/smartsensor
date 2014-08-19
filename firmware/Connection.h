#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include <Arduino.h>
#include <string.h>


class Connection
{
public:
  boolean statusConn;
  boolean modeAT;
  
  void begin();
  void activeModeAT();
  void setConnection();
  boolean verifyConnection();
  boolean checkConnection();
  char* getMAC();
  
private:
};
#endif


