#include "MQTT.h"
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "Secrets.h"



MQTT::MQTT(){
  // WiFi client
  WiFiClientSecure espClient; // Use WiFiClientSecure for TLS
  PubSubClient client(espClient);
  setup();


}

void MQTT::setup() {
  // Skip certificate verification for now (for testing purposes)

  espClient.setInsecure(); // Skip TLS certificate verification for testing
  client.setServer(mqtt_server, mqtt_port);

  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    String clientId = "ESP8266Client-" + String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("MQTT connected!");
    } else {
      Serial.print("MQTT connection failed, rc=");
      Serial.println(client.state());
      Serial.println("Check broker address, credentials, and TLS settings.");
      delay(5000);
    }
  }
}



void MQTT::mqttLoop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void MQTT::reconnect() {
  client.setServer(mqtt_server, mqtt_port);
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("tempGoal");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" trying again in 5 seconds");
      delay(5000);
    }
  }
}

void MQTT::publishMessage(const char* topic, JsonDocument doc, boolean retained) {
  char mqtt_message[256];
  serializeJson(doc, mqtt_message);

  String payload = mqtt_message;

  if (client.publish(topic, payload.c_str(), retained))
    Serial.println("Message published [" + String(topic) + "]: " + payload);
}

// MQTT callback function
void MQTT::callback(char* topic, byte* payload, int length) {
  String incomingMessage = "";
  for (int i = 0; i < length; i++) incomingMessage += (char)payload[i];

  Serial.println("Message arrived [" + String(topic) + "]: " + incomingMessage);

  // Parse incoming message as JSON
  DeserializationError error = deserializeJson(callbackData, incomingMessage);

  if (error) {
    Serial.print("Failed to parse JSON: ");
    Serial.println(error.c_str());
    return;
  }

  // Log parsed JSON data (optional)
  Serial.println("Parsed JSON: ");
  serializeJson(callbackData, Serial);
  Serial.println();
}

// Getter for callback data
JsonDocument MQTT::getCallbackData() {
  return callbackData; // Return the current callback data
}


