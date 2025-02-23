#include "HumidityControl.h"

HumControl::HumControl(){

};

void HumControl::begin(){

    dht.begin();
    pinMode(humPin, OUTPUT);

};

void HumControl::control(){
    float hum = dht.getHumidity();
    float error = humGoal-hum;
    controlSignal = error;

    if (controlSignal > 0){
        this->spray();
    }

};

void HumControl::spray(){
    digitalWrite(humPin, HIGH);
    delay(500);
    digitalWrite(humPin, LOW);
    delay(1000);

};