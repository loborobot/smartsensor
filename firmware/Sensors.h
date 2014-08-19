#ifndef __SENSORS_H__
#define __SENSORS_H__

#include <Arduino.h>
#include <string.h>

class Sensors
{
public:
  float _humidity;
  float _temperature;
  int _valueLDR;
  int _valLDRmin = 1023;
  int _valLDRmax = 0;
  
  float _valueUV;
  float _sound;
  float _flowmeter; 
  float _volume ;
  float _CO2;
  float _NO2;
  
  //char* _valSensor[9];
  boolean dataUpdate;
  
  void begin();
  void execute();
  uint8_t updateSensor();
  char* convertF2C(float val);
  
  uint8_t readDataDHT();
  uint8_t readDataLDR();
  uint8_t readDataUV();
  uint8_t readDataSound();
  uint8_t readDataFlowmeter();
  uint8_t readDataCO2();
  uint8_t readDataNO2();
  volatile int NbTopsFan; //measuring the rising edges of the signal

private:
};
#endif
