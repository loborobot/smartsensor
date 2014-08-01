#ifndef __SENSORS_H__
#define __SENSORS_H__

#include <Arduino.h>
#include <string.h>

class Sensors
{
public:
  void begin();
  void execute();
  
  uint8_t readDataDHT11();
  byte humidity11;
  byte temperature11;
  
  float readDataLDR();
  inline float getValueLDR(){return _lastValueLDR;};
  
  float readDataUV();
  inline float getValueUV(){return _lastValueUV;};
  
  float readDataSound();
  
  volatile int NbTopsFan; //measuring the rising edges of the signal
  void readDataFlowmeter();

private:
  uint8_t _pinLDR;
  float _valueLDR;
  float _lastValueLDR;
  //float _minLDR;
  //float _maxLDR;
  
  uint8_t _pinUV;
  float _valueUV;
  float _lastValueUV;
  //float _minUV;
  //float _maxUV;
  
  float _sound;
 
  float flowmeter; 
  float volume ;
};
#endif
