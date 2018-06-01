#include <Arduino.h>
#include "./batteryMeter.h"

BatteryMeter::BatteryMeter(){}
BatteryMeter::BatteryMeter(int batteryMeterPin){
    this->batteryMeterPin = batteryMeterPin;
}

float BatteryMeter::getBatteryVoltage()
{
    analogReference(INTERNAL);
    delay(50);

    float averageVoltage = 0;

    for (int i = 0; i < this->numberOfvoltageSamples; i++)
    {
        averageVoltage += analogRead(this->batteryMeterPin);
        delay(20);
    }

    averageVoltage = averageVoltage / this->numberOfvoltageSamples;

    analogReference(DEFAULT);
    delay(50);

    return (averageVoltage * this->voltageResolution * (this->voltageDividerResistor_1 + this->voltageDividerResistor_2)) / this->voltageDividerResistor_2;
}