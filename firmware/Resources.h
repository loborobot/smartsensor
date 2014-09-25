#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include <Arduino.h>
#include <string.h>

// class resources: Sdcard, EPROM, RTC
class Resources
{
public:  
  void begin();
  
  String RTCread();
  void writeSD();
  void dateFormat();
  
private:
};

extern Resources RSCS;
#endif
