#include "./utility.h"

Utility::Utility() {}

void Utility::oscillatePin(uint8_t pinNumber, int frequencyInMilliseconds, uint8_t repetitions)
{
    for (uint8_t i = 0; i < repetitions; i++)
    {
        digitalWrite(pinNumber, HIGH);
        delay(frequencyInMilliseconds);
        digitalWrite(pinNumber, LOW);
        delay(frequencyInMilliseconds);
        yield();
    }
}