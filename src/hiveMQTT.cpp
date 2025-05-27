#include "hiveMQTT.h"
#include <Arduino.h>
#include "config.h"

MQTT::MQTT(const char* broker, uint16_t port,
           const char* user,   const char* pass)
  : client_(espClient_),
    broker_(broker),
    port_(port),
    user_(user),
    pass_(pass) {}

void MQTT::begin() {
    espClient_.setInsecure();                        // TLS, skip-verify
    client_.setServer(broker_, port_);
    client_.setCallback([this](char* t, byte* p, unsigned int l) {
        handleMessage_(t, p, l);
    });
}

void MQTT::loop() {
    if (WiFi.status() != WL_CONNECTED) return;

    if (!client_.connected()) {
        uint32_t now = millis();
        if (now - lastReconnectAttempt_ > 3000) {
            lastReconnectAttempt_ = now;
            connectMqtt_();
        }
        return;
    }
    client_.loop();
}

bool MQTT::connectMqtt_() {
    String cid = "colorbot-"STR(COLORBOT_ID)"/status"; //_session-" + String((uint32_t)ESP.getEfuseMac(), HEX);
    String lwt = "machines/" + cid;

    if (client_.connect(cid.c_str(), user_, pass_,
                        lwt.c_str(), 1, true, "offline")) {
        Serial.println("✓ MQTT connected");
        client_.publish(lwt.c_str(), "online", true);
        client_.subscribe("colorbot-"STR(COLORBOT_ID)"/command", 1);
        return true;
    }
    Serial.printf("MQTT connect failed: %d\n", client_.state());
    return false;
}

void MQTT::publish(const char* topic,
                   const JsonDocument& doc,
                   bool retained) {
    char buf[256];
    serializeJson(doc, buf, sizeof(buf));
    client_.publish(topic, buf, retained);
}

void MQTT::handleMessage_(char* topic,
                          uint8_t* payload,
                          unsigned int len) {
    JsonDocument doc;
    if (deserializeJson(doc, payload, len)) return;
    if (userHandler_) userHandler_(topic, doc);
}
