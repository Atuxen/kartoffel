#ifndef DHT11_H
#define DHT11_H

#include "DHTesp.h"
#include "config.h"


class DHT {

    public:
        DHT(); // COnstructor
        void begin();
        float getTemperature(); // Example function to retrieve temperature
        float getHumidity();    // Example function to retrieve humidity
        int dhtpin = DHT_PIN;
        DHTesp dht;

    private:

};

#endif