#ifndef __SENSORS_H__
#define __SENSORS_H__

#include <Arduino.h>

#define SENSORS_NUMBER 11

long value_sensors[SENSORS_NUMBER];

class Sensors
{
public:
  /*float _temperature;
  float _humidity;
  int _valueLDR;
  int _valLDRmin = 1023;
  int _valLDRmax = 0;
  
  float _valueUV;
  float _sound;
  float _flowmeter; 
  float _volume ;
  float _NO2;
  float _CO2;*/
  
  //char* _valSensor[9];
  boolean dataUpdate;
  
  void begin();
  void execute();
 
  uint8_t readDataDHT();
  float getLDR();
  float getUV();
  float getNoise();
  float getFlowMeter();
  float getCO2();
  float getNO2();
  void sensorsUpdate(); 

#if HAS_GPS
  long latLgt[3];
  void getLatLgt(long latLgt[]);
#endif
private:
};
#endif
