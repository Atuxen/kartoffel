#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is connected to GPIO4 (D2 on many boards)
#define ONE_WIRE_BUS D2

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor library
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  Serial.println("DS18B20 Temperature Sensor");

  // Start the DS18B20 sensor
  sensors.begin();
}

void loop() {
  // Request temperature readings from the sensor
  sensors.requestTemperatures();
  
  // Fetch and print the temperature
  float temperatureC = sensors.getTempCByIndex(0);
  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.println("°C");

  // Delay for 2 seconds before taking the next reading
  delay(2000);
}