#ifndef WIFIHELPER_H
#define WIFIHELPER_H

#include "WifiHelper.h"
#include <Arduino.h> // For Serial, delay, etc.

class WifiHelper {
  public:
    // Constructor
    WifiHelper(const char* ssid, const char* password);

    // Method to set up Wi-Fi
    void setupWifi();

  private:
    const char* ssid;
    const char* password;
};

#endif // WIFIHELPER_H
