#include "hiveMQTT.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

// -------------- Constructor -----------------
MQTT::MQTT()
  : espClient(),     // Initializes WiFiClientSecure
    client(espClient) // Pass the espClient to PubSubClient
{

}

// -------------- Setup Connection -----------
void MQTT::setup() {
  // Skip certificate verification (for testing only!)
  espClient.setInsecure();

  // Attach your callback so you can receive messages
  client.setCallback(mqttCallback);

  // Configure server
  client.setServer(mqtt_server, mqtt_port);

  // Attempt initial connection
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    String clientId = "ESP8266Client-" + String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("MQTT connected!");
      // Subscribe to some topic(s) if needed
      client.subscribe("command");
    } else {
      Serial.print("MQTT connection failed, rc=");
      Serial.println(client.state());
      Serial.println("Check broker address, credentials, and TLS settings.");
      delay(5000);
    }
  }
}

// -------------- Handle Loop ---------------
void MQTT::mqttLoop() {
  // If we get disconnected, reconnect
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

// -------------- Reconnect ------------------
void MQTT::reconnect() {
  client.setServer(mqtt_server, mqtt_port);

  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");
      // Re-subscribe if needed
      client.subscribe("command");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" trying again in 5 seconds");
      delay(5000);
    }
  }
}

// -------------- Publish --------------------
void MQTT::publishMessage(const char* topic, JsonDocument doc, boolean retained) {
  char mqtt_message[256];
  serializeJson(doc, mqtt_message); // Convert doc to JSON string

  if (client.publish(topic, mqtt_message, retained)) {
    Serial.print("Message published [");
    Serial.print(topic);
    Serial.print("]: ");
    Serial.println(mqtt_message);
  }
}


void MQTT::publishLoop(TempInternal& object) {
  JsonDocument doc;
  //JsonDocument doc;
  doc["deviceId"] = "Kartoffel chip";
  doc["siteId"] = "Food lab";
  doc[object._sensorName] = object.getSensorData();
  doc["outsideTemperature"] = 20;
  doc["outsideHumidity"] = 30;
  doc["heatSignal"] = 40;
  doc["fanSignal"] = 50;
  doc["humiditySignal"] = 6; 
  publishMessage("test/esp8266", doc, true);

}

// -------------- Static Callback -----------
void MQTT::mqttCallback(char* topic, byte* payload, unsigned int length) {
  // Convert payload to String
  String incomingMessage;
  for (unsigned int i = 0; i < length; i++) {
    incomingMessage += (char)payload[i];
  }

  Serial.println("Message arrived [" + String(topic) + "]: " + incomingMessage);

  // Her skal logikken være for callback

  // Because the callback is static, we need a reference to the *single* MQTT instance if we want to store data
  // Easiest solution: make `callbackData` static or global, or have a global pointer to your MQTT instance.
  // For example, if you have only 1 MQTT object, you can do:
  
  extern MQTT mqtt; // declare in a global .cpp if you want
  
  // Then we can do something like:
  DeserializationError error = deserializeJson(mqtt.callbackData, incomingMessage);
  if (error) {
    Serial.print("Failed to parse JSON: ");
    Serial.println(error.c_str());
    return;
  }

  // Log parsed JSON data (optional)
  Serial.println("Parsed JSON: ");
  serializeJson(mqtt.callbackData, Serial);
  Serial.println();


}


