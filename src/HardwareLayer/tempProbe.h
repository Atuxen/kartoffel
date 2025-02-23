#ifndef TEMPPROBE_H
#define TEMPPROBE_H

#include <OneWire.h>
#include <DallasTemperature.h>
#include "config.h"

class TempProbe {
    public:
        // Constructor
        TempProbe();
    
        // Initialization
        void begin();
    
        // Return the current temperature in °C
        float getSensorData();
    
    
        // Public read-only sensor name
        const char* _sensorName = "Internal temperature probe";
        int oneWireBusPin = ONEWIRE_PIN;
    
    private:
    
        OneWire _oneWire;
        DallasTemperature _sensors;
        
    
        
    
};

#endif
