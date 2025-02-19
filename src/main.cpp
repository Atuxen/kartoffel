// Imports
#include "Secrets.h" // This file is not committed to github, and contains the credentials
#include <ESP8266WiFi.h>
#include "Arduino.h"
#include "WifiHelper.h"

// Sensors and actuators imports
//#include "hiveMQTT.h" // If using HiveMQ as broker 
#include "adafruitMQTT.h" 
#include "TempInternal.h"
//#include "TempExt.h"
//#include "hum.h"

// Object constructions
WifiHelper myWifi(SECRET_SSID, SECRET_PASSWORD); // Passed from Secrets.h
//MQTT mqtt; //Hive mqtt object
adafruitMQTT myAdafruitMQTT; 

TempInternal tempInt(D2);
//TempExt tempExt(); To be implmented
//Humidity hum(); To be implmented

// Variables
static unsigned long lastCheckPub = 0;
int hej = 0;

void setup() {
    Serial.begin(115200);
    myWifi.setupWifi();
    
    delay(1000); 

    tempInt.begin();
    //mqtt.setup(); // For hiveMQ


}

void loop() {
    //mqtt.mqttLoop();
    myAdafruitMQTT.reconnectIfNeeded();
    tempInt.controlLoop();
    
    //JsonDocument json = mqtt.getCallbackData();
    
    // Delayed publish
    if (millis() - lastCheckPub >= 30000) {
      lastCheckPub = millis();
      //mqtt.publishLoop(tempInt); //HiveMQ

      myAdafruitMQTT.publishTemperature(tempInt.getSensorData());
      myAdafruitMQTT.publishHeatMat((tempInt.CONTROL>= 0.5) ? HIGH : LOW);
      

    }

    Serial.print("PID heat control output: ");
    Serial.println(tempInt.CONTROL);
    Serial.print("Temperture probe: ");
    Serial.println(tempInt.getSensorData());


    delay(500);
}

