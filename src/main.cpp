// Imports
#include "Secrets.h" // This file is not committed to github, and contains the credentials
#include <ESP8266WiFi.h>
#include "Arduino.h"
#include "WifiHelper.h"

// Sensors and actuators imports
#include "MQTT.h"
#include "TempInternal.h"
//#include "TempExt.h"
//#include "hum.h"

// Object constructions
WifiHelper myWifi(SECRET_SSID, SECRET_PASSWORD); // Passed from Secrets.h
MQTT mqtt;
TempInternal tempInt;
//TempExt tempExt(); To be implmented
//Humidity hum(); To be implmented

// Variables
static unsigned long lastCheckPub = 0;

void setup() {
    Serial.begin(115200);
    myWifi.setupWifi();
    delay(1000);
    tempInt.begin();
    mqtt.setup();
}

void loop() {
    mqtt.mqttLoop();
    tempInt.controlLoop();
    
    //JsonDocument json = mqtt.getCallbackData();
    
    if (millis() - lastCheckPub >= 5000) {
      lastCheckPub = millis();
      mqtt.publishLoop(tempInt);
    }

    delay(500);

}

