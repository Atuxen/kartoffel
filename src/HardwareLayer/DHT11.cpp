#include "DHT11.h"
DHT::DHT(){
 
};

void DHT::begin(){
    dht.setup(dhtpin, DHTesp::DHT11);
};

float DHT::getTemperature() {
    return dht.getTemperature();
}

float DHT::getHumidity() {
    return dht.getHumidity();
}