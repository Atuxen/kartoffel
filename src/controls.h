#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_TCS34725.h>
#include <SPI.h>

//LED ledStick;  // Define the Qwiic LED Stick

// Define RGB Sensor
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

const char motorPins[6] = {13, 12, 14, 27, 25, 26}; // Pump pins
#define LED_PIN 19
#define NUM_LEDS 4

Adafruit_NeoPixel rgbLed(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Define some colors
struct RGB {
    uint8_t r, g, b;
};

constexpr RGB COLOR_RED = {255, 0, 0};
constexpr RGB COLOR_GREEN = {0, 255, 0};
constexpr RGB COLOR_PURPLE = {255, 0, 255};
constexpr RGB COLOR_WHITE = {255, 145, 36};
constexpr RGB COLOR_OFF = {0, 0, 0};




void setColor(const RGB& color) {
    rgbLed.setPixelColor(0, rgbLed.Color(color.r, color.g, color.b));
    rgbLed.setPixelColor(1, rgbLed.Color(color.r, color.g, color.b));
    rgbLed.setPixelColor(2, rgbLed.Color(color.r, color.g, color.b));
    rgbLed.setPixelColor(3, rgbLed.Color(color.r, color.g, color.b));
    
    rgbLed.show();
}

void pinSetup() {
    // Define Pump PinModes
    for (int i = 0; i < 6; i++) {
        pinMode(motorPins[i], OUTPUT);
        
    }


    //pinMode(8, OUTPUT); // Inactive Pin
    //pinMode(9, OUTPUT); // Inactive Pin
    //
    //digitalWrite(8, HIGH);
    //digitalWrite(9, HIGH);
//
    rgbLed.begin(); // Initialize the NeoPixel library
    setColor(COLOR_RED);
    //rgbLed.show();  // Turn off all LEDs initially

    // FOR TESTING
    // pinMode(13, OUTPUT);
    // digitalWrite(13, LOW);
}

void colorMeasSetup() {

   // // Begin LED and RGB Sensor
   // Wire.begin();
   // ledStick.begin();
   // tcs.begin();
   // tcs.setInterrupt(true); // Turn off sensor light

   setColor(COLOR_RED);
   Serial.println("Color measurement setup complete");
}

void stopPump(int pin) {
    // Deactivate a given pump

    //digitalWrite(pin, LOW); // LOW when LED (FOR TESTING)

    digitalWrite(pin, LOW); // HIGH when relay
    
}

void runPump(int pin, float duration) {
    // Activate a given pump

    //digitalWrite(pin, HIGH); // HIGH when LED (FOR TESTING)

    digitalWrite(pin, HIGH); // LOW when relay
    delay(duration * 1000); 
    stopPump(pin);
}

RGB runMeasurement() {
    setColor(COLOR_WHITE);
    delay(500);

    int n = 10;

    float redSum = 0.0;
    float greenSum = 0.0;
    float blueSum = 0.0;

    float r, g, b;

    for (int i = 0; i < n; i++) {
        tcs.getRGB(&r, &g, &b);  // Read into temporary values
        redSum += r;
        greenSum += g;
        blueSum += b;
        delay(100);
    }

    RGB colormeasured = {
        static_cast<uint8_t>(redSum / n),
        static_cast<uint8_t>(greenSum / n),
        static_cast<uint8_t>(blueSum / n)
    };

    setColor(COLOR_OFF);
    delay(1000);

    Serial.print("<RGB:");
    Serial.print(int(colormeasured.r));
    Serial.print(",");
    Serial.print(int(colormeasured.g));
    Serial.print(",");
    Serial.print(int(colormeasured.b));
    Serial.println(">");

    return colormeasured;
}



