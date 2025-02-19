#include "TempExt.h"
#include <ESP8266WiFi.h>
#include <Arduino.h> 
#include "DHTesp.h"


TempExt::TempExt(int DHTpin){
    DHTesp dht;
    pinMode(DHTpin, INPUT);
    dht.setup(DHTpin, DHTesp::DHT11);


}