#ifndef TEMPINTERNAL_H
#define TEMPINTERNAL_H

#include <OneWire.h>
#include <DallasTemperature.h>

class TempInternal {
public:
    // Constructor
    TempInternal(int oneWireBusPin);

    // Initialization
    void begin();

    // Return the current temperature in °C
    float getSensorData();

    // Simple PID control loop
    void controlLoop();

    // Public read-only sensor name
    const char* _sensorName = "Internal temperature probe";

    float CONTROL = 0.0;
    float _goalTemp = 100.0;

private:
    // Pin assignments
    //int _oneWireBusPin = D2;
    int _heatMatPin    = D3;

    // OneWire & DallasTemperature
    OneWire _oneWire;
    DallasTemperature _sensors;

    // PID parameters
    
    float _kp = 2.0, _ki = 0.5, _kd = 1.0;
    float _integral = 0.0, _lastError = 0.0;
    

};

#endif
