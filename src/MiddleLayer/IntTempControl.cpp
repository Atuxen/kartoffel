#include "IntTempControl.h"
#include <Arduino.h> // for pinMode, digitalWrite, etc.

InTempControl::InTempControl(){
};

void InTempControl::begin()
{

    probe.begin();
    pinMode(heatMatPin, OUTPUT);
};

void InTempControl::control() {
  float currentTemp = probe.getSensorData();
  
  float error = goalTemp - currentTemp;
  controlSignal = error;

  if (controlSignal >= 0){
    digitalWrite(heatMatPin, HIGH);
  }
    
  else {
    digitalWrite(heatMatPin, LOW);
  }

}




