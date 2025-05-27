#include <Arduino.h>
#include <Wire.h>
#include "comms.h"

#include <Adafruit_TCS34725.h>
#include <SPI.h>



void setup() {
  Serial.begin(9600);
  Serial.println("Setup started");
  Serial.println("<Colorbot is ready>"); // Tell PC we are ready

  Serial.println("Before pinSetup");
  pinSetup();
  Serial.println("After pinSetup");

  Serial.println("Before colorMeasSetup");
  colorMeasSetup();
  Serial.println("After colorMeasSetup");

  Serial.println("Setup complete");

  
}

void loop() {
  curMillis = millis();
  getDataFromPC();
  // replyToPC();
}

