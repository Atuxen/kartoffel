#ifndef ADAFRUITMQTT_H
#define ADAFRUITMQTT_H

#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "Secrets.h" // For AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY

class adafruitMQTT {
  public:
    adafruitMQTT();               // constructor

    // Call this in your loop to ensure MQTT is connected
    void reconnectIfNeeded();

    // Publish a temperature value (example)
    void publishTemperature(float value);

    void publishHeatMat(float value);

    void publishHumidity(float value);


    void subscribeCommand();
    float handleMQTT();  // Function to process incoming messages

  
    // The underlying TCP client
    WiFiClient client;

    // The Adafruit MQTT client
    Adafruit_MQTT_Client mqtt;

    // Publishers
    Adafruit_MQTT_Publish temperatureFeed;
    Adafruit_MQTT_Publish heatMatFeed;
    Adafruit_MQTT_Publish DHT11;
    Adafruit_MQTT_Publish humidifier;
    Adafruit_MQTT_Publish extTemp;
    



    // Subscribers
    Adafruit_MQTT_Subscribe command;
  private:

};

#endif
