// Component imports
#include "Secrets.h" // This file is not committed to github, and contains the credentials
#include <ESP8266WiFi.h>
#include "Arduino.h"
#include "MiddleLayer/WifiHelper.h"

// Middleware imports
#include "MiddleLayer/adafruitMQTT.h" 
#include "MiddleLayer/IntTempControl.h"
#include "MiddleLayer/HumidityControl.h"

// Object constructions
WifiHelper myWifi(SECRET_SSID, SECRET_PASSWORD); // Passed from Secrets.h
adafruitMQTT myAdafruitMQTT; 
InTempControl Temp;
HumControl Hum;

// Variables
static unsigned long lastCheckPub = 0;


void setup() {
    Serial.begin(115200);
    myWifi.setupWifi();
    
    delay(1000); 

    Temp.begin();
    Hum.begin();


}

void loop() {

    myAdafruitMQTT.reconnectIfNeeded();
    Temp.control();
    Hum.control();

    // To be implemented in adafruit lib
    float value = myAdafruitMQTT.handleMQTT();        // Process any incoming messages
    if (value != -100){
        Temp.goalTemp = value;
    }
    
    // Delayed publish
    if (millis() - lastCheckPub >= 30000) {
      lastCheckPub = millis();

      //myAdafruitMQTT.publishTemperature(Temp.probe.getSensorData());
      //myAdafruitMQTT.publishHeatMat((Temp.controlSignal> 0) ? HIGH : LOW);
      myAdafruitMQTT.temperatureFeed.publish(Temp.probe.getSensorData());
      myAdafruitMQTT.heatMatFeed.publish((Temp.controlSignal> 0) ? HIGH : LOW);
      myAdafruitMQTT.DHT11.publish(Hum.dht.getHumidity());
      myAdafruitMQTT.extTemp.publish(Hum.dht.getTemperature());
      myAdafruitMQTT.humidifier.publish((Hum.controlSignal> 0) ? HIGH : LOW);

    }

    Serial.println("DHT data Humidity");
    Serial.println(Hum.dht.getHumidity());
    Serial.println("DHT data temperature");
    Serial.println(Hum.dht.getTemperature());

    Serial.print("PID heat control output: ");
    Serial.println(Temp.controlSignal);
    Serial.print("Temperture probe: ");
    Serial.println(Temp.probe.getSensorData());
    
    delay(500);
}

