#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include <Arduino.h>
#include <string.h>


class Connection
{
public:
  void begin();
  uint8_t send();
  void read();
  
private:
  boolean notConnected;

};
#endif

