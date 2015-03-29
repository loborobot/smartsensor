#ifndef __CORE_H__
#define __CORE_H__

//#include <Arduino.h>

// class resources: Sdcard, EPROM, RTC
class Core
{
public:  
  void begin();
  
  String getRTC();
  void writeSD();
  void dateFormat();
  
private:
};
#endif
