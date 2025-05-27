#pragma once
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <functional>

class MQTT {
public:
    using MsgHandler =
        std::function<void(const char*, const JsonDocument&)>;

    MQTT(const char* broker, uint16_t port,
         const char* user,   const char* pass);

    void begin();             // call once in setup()
    void loop();              // call in main loop()
    void publish(const char* topic,
                 const JsonDocument& doc,
                 bool retained = false);

    void onMessage(MsgHandler cb) { userHandler_ = std::move(cb); }

private:
    bool connectMqtt_();
    void handleMessage_(char* topic, uint8_t* payload, unsigned int len);

    WiFiClientSecure espClient_;
    PubSubClient     client_;

    const char* broker_;
    uint16_t     port_;
    const char*  user_;
    const char*  pass_;

    MsgHandler   userHandler_;
    uint32_t     lastReconnectAttempt_ = 0;
};
