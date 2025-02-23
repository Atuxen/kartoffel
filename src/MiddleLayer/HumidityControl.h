#ifndef HUMIDITYCONTROL_H
#define HUMIDITYCONTROL_H

#include "config.h"
#include "Arduino.h"
#include "HardwareLayer/DHT11.h"

class HumControl {

    public:
        HumControl(); // Constructor
        void begin();
        void control();
        void spray();
        DHT dht;
        int humPin = HUMIDIFIER_PIN;
        float humGoal = 70.0;
        float controlSignal;
        



    private:

};


#endif


