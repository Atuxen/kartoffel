#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>



#include "Secrets.h" // This file is not committed
#include "mqttSetup.h"
#include "WifiHelper.h"  // Include your custom class
#include "pidControl.h"

// Pins


#define FAN D6
#define DIFFUSER D7
#define HEAT D3

// Variables

float outsideTemperature;
float outsideHumidity;

float heatSignal;
float fanSignal;
float humiditySignal;








bool IOT = true;



void setup() {
    // Pin mode setup
  
  pinMode(FAN, OUTPUT);
  pinMode(DIFFUSER, OUTPUT);
  pinMode(HEAT, OUTPUT);

}



void loop() {

  if (IOT){
    
  }


    // Access JSON data from callback every second
  static unsigned long lastCheckCall = 0;
  static unsigned long lastCheckPub = 0;

  

  if (IOT){
    if (millis() - lastCheckCall >= 10000) {
      lastCheckCall = millis();

      // Get the latest callback data
      JsonDocument jsonData = getCallbackData();

      // Access specific fields (e.g., temperature goal)
      if (jsonData.containsKey("tempGoal")) {
        internalTempSet = jsonData["tempGoal"];
        Serial.println("Temperature goal: " + String(internalTempSet));
      } else {
        Serial.println("No 'tempGoal' in callback data");
      }
    }
  }

  if (IOT){
    if (millis() - lastCheckPub >= 5000) {
      lastCheckPub = millis();
      Serial.println("Now publishing");
      JsonDocument doc;
      doc["deviceId"] = "Kartoffel chip";
      doc["siteId"] = "Food lab";
      doc["internalTemperature"] = internalTemperature;
      doc["outsideTemperature"] = outsideTemperature;
      doc["outsideHumidity"] = outsideHumidity;
      doc["heatSignal"] = heatSignal;
      doc["fanSignal"] = fanSignal;
      doc["humiditySignal"] = humiditySignal;

      publishMessage("test/esp8266", doc, true);
    }
  }

  sensors.requestTemperatures();                // Request temperature readings
  
  if (sensors.getTempCByIndex(0) != DEVICE_DISCONNECTED_C) {         // Check if reading was successful
    internalTemperature = sensors.getTempCByIndex(0);     // Get the temperature in Celsius
    Serial.print("Temperature: ");
    Serial.print(internalTemperature);
    Serial.println(" °C");
  }   else {
    Serial.println("Error: Temp reading");
  }

  /*internalTemperature = random(20,30);
  outsideHumidity = random(20,30);
  outsideTemperature = random(20,30);
  */
  
  

  delay(dht.getMinimumSamplingPeriod());

  float outsideHumidity = dht.getHumidity();
  float outsideTemperature = dht.getTemperature();


  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(outsideHumidity, 1);
  Serial.print("\t\t");
  Serial.print(outsideTemperature, 1);
  Serial.print("\t\t");


  heatSignal = pidControlTemp(internalTemperature, internalTempSet);
  humiditySignal = pidControlHum(outsideHumidity, outsideHumiditySet);
  fanSignal = humiditySignal;

}


