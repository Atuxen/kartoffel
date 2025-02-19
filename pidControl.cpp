#include "pidControl.h"
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

// PID control parameters
float setpoint;  // Desired temperature in °C
float kp = 2.0, ki = 0.5, kd = 1.0; // PID coefficients
float integral = 0, lastError = 0;


float pidControlTemp(float currentTemp, float setpoint) {
  float error = setpoint - currentTemp;
  integral += error;
  float derivative = error - lastError;

  float output = kp * error + ki * integral + kd * derivative;
  lastError = error;

  // Clamp output to control signal range (0-255 for PWM)
  output = constrain(output, 0, 255);
  Serial.println("PID heat control output: " + String(output));

  return output;
 
}


float pidControlHum(float currentHum, float setpoint)  {
  float error = setpoint - currentHum;
  integral += error;
  float derivative = error - lastError;

  float output = kp * error + ki * integral + kd * derivative;
  lastError = error;

  // Clamp output to control signal range (0-255 for PWM)
  output = constrain(output, 0, 255);
  Serial.println("PID humidity control output: " + String(output));

  return output;
 
}


