#include <Arduino.h>

#include "WaterPump.h"

WaterPump::WaterPump() {}

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
    // Serial.println(this->newWateringTimeInSeconds);
    digitalWrite(this->waterPumpPin, LOW);
    return true;
}

void WaterPump::updateWateringTime(int &newWateringTimeInSeconds)
{
    this->newWateringTimeInSeconds = newWateringTimeInSeconds;
    // Serial.print("Watering time in seconds: ");
    // Serial.println(this->newWateringTimeInSeconds);
}