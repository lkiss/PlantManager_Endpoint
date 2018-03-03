#include <Arduino.h>

#include "WaterPump.h"

WaterPump::WaterPump(){}

WaterPump::WaterPump(int *waterPumpPin)
{
    pinMode(*waterPumpPin, OUTPUT);
    digitalWrite(*waterPumpPin, LOW);
    this->waterPumpPin = *waterPumpPin;
}

bool WaterPump::activateWaterPump()
{
    //Log debug
    digitalWrite(this->waterPumpPin, HIGH);
    delay(this->newWateringTimeInSeconds * 1000);
    digitalWrite(this->waterPumpPin, LOW);
    return true;
}

void WaterPump::updateWateringTime(int newWateringTimeInSeconds){
    this->newWateringTimeInSeconds = newWateringTimeInSeconds;
}