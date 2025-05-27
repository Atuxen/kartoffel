#include "Secrets.h"
#include "WifiHelper.h"
#include "hiveMQTT.h"
#include <ArduinoJson.h>
#include "config.h"

#include <Arduino.h>
#include <Wire.h>
#include "comms.h"

#include <Adafruit_TCS34725.h>
#include <SPI.h>


WifiHelper wifi;
MQTT mqtt(MQTT_SERVER, 8883, MQTT_USRNAME, MQTT_PASSWORD);
bool usbConnected = false;

//constexpr int LED_PIN = 2;              // NodeMCU-32S on-board LED

void waitForWiFi() {
      while (!wifi.setupWifi()) {
          Serial.println("Retrying Wi-Fi in 2 seconds…");
          delay(2000);
      }
  }

void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.println("Setup started");

    pinSetup();
    colorMeasSetup();
    Serial.println("Setup complete");

    // Wait up to 3 seconds for serial to be connected
    unsigned long detectStart = millis();
    while (millis() - detectStart < 3000) {
      if (Serial.available()) {
        usbConnected = true;
        break;
    }
  }
    usbConnected = Serial.available() > 0;
    
    usbConnected = false; // Temporary workaround: forces WiFi mode as USB detection logic is not working. Replace with proper USB detection logic.

    if (usbConnected) {
        setColor(COLOR_GREEN);
        Serial.println("USB serial detected — skipping WiFi");
        unsigned long lastReadyMsg = 0;

        while (true) {
              // Print "<Colorbot is ready>" once per second
          if (millis() - lastReadyMsg > 1000) {
              Serial.println("<Colorbot is ready>");
              lastReadyMsg = millis();
          }
          if (Serial.available()) {
            String input = Serial.readStringUntil('\n');
            input.trim();
            if (input == "CONTINUE") {
              Serial.println("Received CONTINUE. Proceeding...");
              setColor(COLOR_WHITE);
              break;
            } else {
              Serial.println("Unknown input. Please send 'CONTINUE'");
            }
          }
        delay(100);
        }
        
    } else {
        Serial.println("No USB detected — going to WiFi mode");

        waitForWiFi();  
        setColor(COLOR_GREEN);
        mqtt.begin();
        setColor(COLOR_PURPLE);
    }

    mqtt.onMessage([](const char* topic, const JsonDocument& doc) {
      String reqId = "";
      if (strcmp(topic, "colorbot-" STR(COLORBOT_ID) "/command") == 0) {
          if (doc.containsKey("req_id")) reqId = doc["req_id"].as<const char*>();
          // task may be a string (like "Meas") or a stringified JSON (like "{\"Mix\":12,\"duration\":1}")
          String taskStr = doc["task"].as<const char*>();
          JsonDocument taskDoc;
          bool taskIsJson = false;
          DeserializationError err = deserializeJson(taskDoc, taskStr);
          if (!err) {
              taskIsJson = true;
          }
          // If task is just "Meas"
          if (!taskIsJson && taskStr == "Meas") {
              RGB sensed = runMeasurement();
              JsonDocument out;
              if (reqId != "") out["req_id"] = reqId;
              JsonObject clr = out.createNestedObject("color_sense");
              clr["r"] = sensed.r;
              clr["g"] = sensed.g;
              clr["b"] = sensed.b;
              String topic = "colorbot-"STR(COLORBOT_ID)"/data";
              mqtt.publish(topic.c_str(), out, false);
          }
          // If task is a JSON object (e.g., Mix)
          else if (taskIsJson && taskDoc.containsKey("Mix")) {
              if (taskDoc.containsKey("Mix") && taskDoc.containsKey("duration")) {
                  int pin = taskDoc["Mix"];
                  float duration = taskDoc["duration"];
                  runPump(pin, duration);
                  // Acknowledge command performed
                  JsonDocument out;
                  if (reqId != "") out["req_id"] = reqId;
                  out["status"] = "Mix performed";
                  String topic = "colorbot-"STR(COLORBOT_ID)"/data";
                  mqtt.publish(topic.c_str(), out, false);
              } else {
                  // Handle missing keys
                  JsonDocument out;
                  if (reqId != "") out["req_id"] = reqId;
                  out["status"] = "Error: Missing 'Mix' or 'duration' key";
                  String topic = "colorbot-"STR(COLORBOT_ID)"/data";
                  mqtt.publish(topic.c_str(), out, false);
              }
          }
      }
        
      else {
            // Unknown command, reply with error
            JsonDocument out;
            if (reqId != "") out["req_id"] = reqId;
            out["status"] = "Unknown command";
            String topic = "colorbot-"STR(COLORBOT_ID)"/data";
            mqtt.publish(topic.c_str(), out, false);
        }
      });
}

void loop() {
  curMillis = millis();
  getDataFromPC();
  // replyToPC();
//

if (!usbConnected) {
  mqtt.loop();
}
//  static unsigned long last = 0;
//  if (millis() - last > 10000) {
//      last = millis();
//      JsonDocument doc;
//      doc["temp"] = 23.4;
//      doc["hum"]  = 41;
//      mqtt.publish("colorbot-"STR(COLORBOT_ID)"/data", doc, false);
//  }
 

}

