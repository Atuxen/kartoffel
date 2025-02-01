#include "TempInternal.h"
#include <Arduino.h> // for pinMode, digitalWrite, etc.

TempInternal::TempInternal()
  : _oneWire(_oneWireBusPin),
    _sensors(&_oneWire)
{
  // Nothing else needed here
}

void TempInternal::begin() {
  _sensors.begin();
  pinMode(_heatMatPin, OUTPUT);
  digitalWrite(_heatMatPin, LOW); // start off
}

float TempInternal::getSensorData() {
  _sensors.requestTemperatures();
  return _sensors.getTempCByIndex(0); 
}

void TempInternal::controlLoop() {
  float currentTemp = getSensorData();
  float error       = _goalTemp - currentTemp;

  _integral += error;
  float derivative = error - _lastError;
  float CONTROL    = _kp * error + _ki * _integral + _kd * derivative;
  _lastError = error;

  // If truly PWM from 0–255, do:
  CONTROL = constrain(CONTROL, 0, 255);

  Serial.print("PID heat control output: ");
  Serial.println(CONTROL);

  digitalWrite(_heatMatPin, (CONTROL >= 128) ? HIGH : LOW);
  
}
