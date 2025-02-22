#include "adafruitMQTT.h"
#include "Arduino.h"  // For Serial prints, delay, etc.

adafruitMQTT::adafruitMQTT()
  : mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY),
    temperatureFeed(&mqtt, AIO_USERNAME "/feeds/temperature"),
    heatMatFeed(&mqtt, AIO_USERNAME "/feeds/heatMatControl"),
    command(&mqtt, AIO_USERNAME "/feeds/command") // Subscribe to heat mat control feed
{
  // Constructor body is empty since we're initializing in the list above
  // but you could put debug prints here if you want.
}

void adafruitMQTT::subscribeCommand() {
  mqtt.subscribe(&command);
}


float adafruitMQTT::handleMQTT() {
  Adafruit_MQTT_Subscribe *subscription;
  float value = -100;
  while ((subscription = mqtt.readSubscription(1000))) {
    if (subscription == &command) {
      Serial.print("Received heat mat control signal: ");
      Serial.println((char *)command.lastread);
      
      // Convert received value to float if needed
      value = atof((char *)command.lastread);
      
      // Handle the received value (turn on/off a relay, etc.)

    }
  }
  
  return value;
}


void adafruitMQTT::reconnectIfNeeded() {
  if (!mqtt.connected()) {
    Serial.println("AdafruitMQTT: Not connected to MQTT, trying to connect...");
    int8_t ret;
    // Retry until successful
    while ((ret = mqtt.connect()) != 0) {
      mqtt.disconnect();
      Serial.print("MQTT connect failed (code ");
      Serial.print(ret);
      Serial.println("). Retry in 5 seconds...");
      delay(5000);
    }
    Serial.println("AdafruitMQTT: Connected!");
  }
}

void adafruitMQTT::publishTemperature(float value) {
  if (!temperatureFeed.publish(value)) {
    Serial.println(F("Failed to publish temperature"));
  } else {
    Serial.print(F("Published temperature: "));
    Serial.println(value);
  }
}

void adafruitMQTT::publishHeatMat(float value) {
  if (!heatMatFeed.publish(value)) {
    Serial.println(F("Failed to publish heat mat control signal"));
  } else {
    Serial.print(F("Published heat mat control signal: "));
    Serial.println(value);
  }
}

