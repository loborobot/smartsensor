#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include <Arduino.h>
#include <string.h>


class Resources
{
public:
  void begin();
  void execute();
  
  void RTCread();
  void writeSD();
  
private:
  

};

extern Resources RSCS;
#endif
