#ifndef __SENSORS_H__
#define __SENSORS_H__

#include <Arduino.h>

#define SENSORS_NUMBER 8


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
  
  boolean dataUpdate;
  
  void begin();
  void execute();
 
  //uint8_t readDataDHT();
  boolean readDHT(void);
  //float readTemperature();
  //float readHumidity();
  float getLDR();
  float getUV();
  float getNoise();
  float getFlowMeter();
  float getCO();
  float getNO2();
  void sensorsUpdate(); 
  void readSerials();
  void terminal();
  //long latLgt[3];
  float avg(byte pin);

  //void getLatLgt(long *latLgt);
private:
};
#endif
