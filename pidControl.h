#ifndef PIDCONTROL_H
#define PIDCONTROL_H





float pidControlTemp(float currentTemp, float setpoint);
float pidControlHum(float currentHum, float setpoint);

#endif