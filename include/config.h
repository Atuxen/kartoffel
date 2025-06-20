#ifndef CONFIG_H
#define CONFIG_H

#define DHT_PIN D5
#define HUMIDIFIER_PIN D6

#define HEAT_MAT_PIN D3
#define FAN_PIN D1
#define ONEWIRE_PIN D2

#define COLORBOT_ID 4               // numeric literal

#define STR_(x)  #x                 // helper: stringify token
#define STR(x)   STR_(x)

#endif 
