#ifndef __RESOURCES_H__
#define __RESOURCES_H__

//#include <Arduino.h>

// class resources: Sdcard, EPROM, RTC
class Core
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
