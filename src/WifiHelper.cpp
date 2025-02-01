#include "WifiHelper.h"
#include <ESP8266WiFi.h>  // <-- For ESP8266 Wi-Fi functions

// Constructor definition
WifiHelper::WifiHelper(const char* ssid, const char* password) {
  this->ssid = ssid;
  this->password = password;
}

// Implementation of setupWifi
void WifiHelper::setupWifi() {
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // For ESP8266, using STATION mode:
  WiFi.mode(WIFI_STA);         
  WiFi.begin(ssid, password);  // Start Wi-Fi

  // Wait until connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress()); 
}
