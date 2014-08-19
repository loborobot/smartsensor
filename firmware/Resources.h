#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include <Arduino.h>
#include <string.h>


class Resources
{
public:
  String data;
  
  void begin();
  //void execute();
  
  void RTCread();
  void writeSD();
  void sendQuery();
  void sendData();
  int strucData();
  
private:
  

};

extern Resources RSCS;
#endif
