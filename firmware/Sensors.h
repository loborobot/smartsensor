#ifndef __SENSORS_H__
#define __SENSORS_H__

#include <Arduino.h>
#include <string.h>

class Sensors
{
public:
  void begin();
  void execute();
  void updateSensor();
  
  uint8_t readDataDHT();
  uint8_t readDataLDR();
  uint8_t readDataUV();
  uint8_t readDataSound();
  uint8_t readDataFlowmeter();
  volatile int NbTopsFan; //measuring the rising edges of the signal

private:
  float _humidity;
  float _temperature;
  int _valueLDR;
  int _valLDRmin = 1023;
  int _valLDRmax = 0;
  float _valueUV;
  float _sound;
  float _flowmeter; 
  float _volume ;
};
#endif
