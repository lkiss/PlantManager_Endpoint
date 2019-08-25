#include <Arduino.h>

#include "WaterPump.h"

WaterPump::WaterPump() {}

WaterPump::WaterPump(int waterPumpIndex, int statusLedPin)
{
    this->waterPumpIndex = waterPumpIndex;
    this->statusLedPin = statusLedPin;
}

bool WaterPump::activateWaterPump()
{
    // Serial.println("Before activate");
    bool indicateWatering = this->statusLedPin != -1 ? true : false;

    indicateWatering ? digitalWrite(this->statusLedPin, HIGH) : digitalWrite(this->statusLedPin, LOW);
    // Serial.println("Before analogWrite");
    pinMode(MUX_COMMON_PIN, OUTPUT);
    analogWrite(MUX_COMMON_PIN, ANALOG_MAX);
    // Serial.println("After analogWrite");
    // Serial.println(this->newWateringTimeInSeconds);
    delay(this->newWateringTimeInSeconds * 1000);
    // Serial.println("Before analogWrite");
    analogWrite(MUX_COMMON_PIN, ANALOG_MIN);
    // Serial.println("After analogWrite");

    indicateWatering ? digitalWrite(this->statusLedPin, LOW) : digitalWrite(this->statusLedPin, LOW);
    // Serial.println("After activate");
    return true;
}

void WaterPump::updateWateringTime(int &newWateringTimeInSeconds)
{
    this->newWateringTimeInSeconds = newWateringTimeInSeconds;
    // Serial.print("Watering time in seconds: ");
    // Serial.println(this->newWateringTimeInSeconds);
}