

//Arduino Code
//Code by LoboRobotElectronics company
//Version 1.0 - 2014
// autor ; Steve Ataucuri Cruz

#include <Wire.h>
#include <RTClib.h>
#include <WiFlyHQ.h>
#include "Sensors.h"

Sensors sensors;

void setup () {  
  sensors.begin();
}

void loop () {
   sensors.execute();
}
