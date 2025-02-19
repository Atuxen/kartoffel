#include "TempInternal.h"
#include <Arduino.h> // for pinMode, digitalWrite, etc.

TempInternal::TempInternal(int oneWireBusPin)
  : _oneWire(oneWireBusPin), _sensors(&_oneWire) // ✅ Correct initialization
{
}

void TempInternal::begin() {
  _sensors.begin();
  int deviceCount = _sensors.getDeviceCount();
  Serial.print("OneWire Devices Found: ");
  Serial.println(deviceCount);
  
  if (deviceCount == 0) {
      Serial.println("⚠️ ERROR: No OneWire sensors found! Check wiring.");
  }

  pinMode(_heatMatPin, OUTPUT);
  digitalWrite(_heatMatPin, LOW);
}

float TempInternal::getSensorData() {
  _sensors.requestTemperatures();
  float temp = _sensors.getTempCByIndex(0);

  if (temp == -127.0) {
    Serial.println("⚠️ ERROR: Sensor not responding. Check wiring!");
  }

  return temp;
}

void TempInternal::controlLoop() {
  float currentTemp = getSensorData();
  
  float error = _goalTemp - currentTemp;
  Serial.println(error);
 
  CONTROL = error;
  Serial.println(CONTROL);
  _lastError = error;

  if (error > 0.5){
    digitalWrite(_heatMatPin, HIGH);
  }

  else {
    digitalWrite(_heatMatPin, LOW);
  }
  
} 


