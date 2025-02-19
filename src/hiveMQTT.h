#ifndef HIVEMQTT_H
#define HIVEMQTT_H

#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "Secrets.h"
#include <ESP8266WiFi.h>  // For WiFiClientSecure, etc.
#include "TempInternal.h"

class MQTT {
public:
    MQTT();

    void setup();
    void publishMessage(const char* topic, JsonDocument doc, boolean retained);
    void mqttLoop();
    void reconnect();
    void publishLoop(TempInternal& object);
    
private:
    // Callback that PubSubClient will call on incoming messages
    static void mqttCallback(char* topic, byte* payload, unsigned int length);

    // HiveMQ or other broker details
    const char* mqtt_server   = MQTT_SERVER;
    const char* mqtt_username = MQTT_USRNAME;
    const char* mqtt_password = MQTT_PASSWORD;
    const int   mqtt_port     = MQTT_PORT;

    // The actual WiFiClientSecure (TLS) object
    WiFiClientSecure espClient;
    // The PubSubClient that references our WiFiClientSecure
    PubSubClient client;


    // Dynamic JSON doc to store incoming data
    // NOTE: For smaller messages, 512 is often enough
    JsonDocument callbackData;
};

#endif // MQTT_H
