#ifndef INTTEMPCONTROL_H
#define INTTEMPCONTROL_H

#include <OneWire.h>
#include <DallasTemperature.h>
#include "config.h"
#include "HardwareLayer/tempProbe.h"

class InTempControl {
    public:
        // Constructor
        InTempControl();

        void begin();
        // Simple P control loop
        void control();
    
        TempProbe probe;

        float goalTemp = 30.0;
        int heatMatPin = HEAT_MAT_PIN;
        float controlSignal;


    private:

};

#endif
