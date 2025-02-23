#include "tempProbe.h"
#include <Arduino.h> // for pinMode, digitalWrite, etc.

TempProbe::TempProbe()
  : _oneWire(oneWireBusPin), _sensors(&_oneWire) // Correct initialization
{
}

void TempProbe::begin() {
  _sensors.begin();
  int deviceCount = _sensors.getDeviceCount();
  Serial.print("OneWire Devices Found: ");
  Serial.println(deviceCount);
  
  if (deviceCount == 0) {
      Serial.println("ERROR: No OneWire sensors found! Check wiring.");
  }

}

float TempProbe::getSensorData() {
  _sensors.requestTemperatures();
  float temp = _sensors.getTempCByIndex(0);

  if (temp == -127.0) {
    Serial.println("ERROR: Sensor not responding. Check wiring!");
  }

  return temp;
}



