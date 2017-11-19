#include "./waterLevelSensor.h"
#include <Arduino.h>

WaterLevelSensor::WaterLevelSensor() {}

WaterLevelSensor::WaterLevelSensor(int triggerPin, int echoPin)
{
    pinMode(echoPin, INPUT);
    pinMode(triggerPin, OUTPUT);

    this->triggerPin = triggerPin;
    this->echoPin = echoPin;
}

long WaterLevelSensor::getMissingWaterColumHeighCM()
{
    long duration, distance = 0;
    int maximumRange = 200;
    int minimumRange = 0;

    digitalWrite(this->triggerPin, LOW);
    delayMicroseconds(2);

    digitalWrite(this->triggerPin, HIGH);
    delayMicroseconds(10);

    digitalWrite(this->triggerPin, LOW);
    duration = pulseIn(this->echoPin, HIGH);

    distance = duration / 58.2;

    if (distance >= maximumRange || distance <= minimumRange)
    {
        distance = -1;
    }

    delay(50);
    return distance;
}