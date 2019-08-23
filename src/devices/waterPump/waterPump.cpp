#include <Arduino.h>

#include "WaterPump.h"

WaterPump::WaterPump() {}

WaterPump::WaterPump(int waterPumpPin)
{
    this->waterPumpPin = waterPumpPin;
    setupWaterPump();
}

WaterPump::WaterPump(int waterPumpIndex, int statusLedPin)
{
    this->waterPumpIndex = waterPumpIndex;
    this->statusLedPin = statusLedPin;
    setupWaterPump();
}

bool WaterPump::activateWaterPump()
{
    bool indicateWatering = this->statusLedPin != -1 ? true : false;

    indicateWatering ? digitalWrite(this->statusLedPin, HIGH) : digitalWrite(this->statusLedPin, LOW);

    digitalWrite(this->waterPumpPin, HIGH);
    delay(this->newWateringTimeInSeconds * 1000);
    // Serial.println(this->newWateringTimeInSeconds);
    digitalWrite(this->waterPumpPin, LOW);

    indicateWatering ? digitalWrite(this->statusLedPin, LOW) : digitalWrite(this->statusLedPin, LOW);
    return true;
}

void WaterPump::updateWateringTime(int &newWateringTimeInSeconds)
{
    this->newWateringTimeInSeconds = newWateringTimeInSeconds;
    // Serial.print("Watering time in seconds: ");
    // Serial.println(this->newWateringTimeInSeconds);
}

void WaterPump::setupWaterPump()
{
    pinMode(this->waterPumpPin, OUTPUT);
    digitalWrite(this->waterPumpPin, LOW);
}