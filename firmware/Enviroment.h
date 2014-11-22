#ifndef __SENSORS_H__
#define __SENSORS_H__

#include <Arduino.h>
#include <string.h>

// class sensors
class Enviroment
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
  void execute(int flo, int vol);
  String  strucData(int flo, int vol);
  char* convertF2C(float val);
  
  uint8_t readDataDHT();
  int readDataLDR();
  char* readDataUV();
  char* readDataSound();
  char* readDataFlowmeter(int flat);
  char* readDataCO2();
  char* readDataNO2();
  volatile int NbTopsFanSen; //measuring the rising edges of the signal

private:
};
#endif
