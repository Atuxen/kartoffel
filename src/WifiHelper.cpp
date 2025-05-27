#include "WifiHelper.h"
#include "Secrets.h"
#include <WiFi.h>

WifiHelper::WifiHelper() {}

bool WifiHelper::setupWifi() {
    Serial.println("Attempting to connect to known WiFi networks…");

    WiFi.mode(WIFI_STA);

    for (int i = 0; i < wifiCredentialsCount; ++i) {
        const char* ssid     = wifiCredentials[i].ssid;
        const char* password = wifiCredentials[i].password;

        Serial.printf("  → %s\n", ssid);
        WiFi.begin(ssid, password);

        for (int retries = 0; retries < 20 && WiFi.status() != WL_CONNECTED; ++retries) {
            delay(250);
            Serial.print('.');
        }

        if (WiFi.status() == WL_CONNECTED) {
            Serial.printf("\n✓ Wi-Fi connected  %s  %s\n",
                          ssid, WiFi.localIP().toString().c_str());
            return true;                       // ---------- success
        }
        Serial.println("\n  × failed");
    }
    Serial.println("No known networks reachable.");
    return false;                              // ---------- gave up
}
