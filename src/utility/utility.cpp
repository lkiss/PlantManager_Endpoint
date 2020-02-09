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

void Utility::sendMessageAndWait(String message)
{
    String response;
    Serial.println("-" + message + "-");
    Serial.flush();
    while (true)
    {
        while (!Serial.available())
        {
            yield();
        }

        while (Serial.available())
        {
            char readCharacter = Serial.read();

            if (readCharacter == '-')
            {
                while (Serial.available())
                {
                    if (Serial.read() == '-')
                    {
                        break;
                    }
                    response += Serial.read();
                }
            }
        }

        response.trim();

        // Serial.print(response);
        // Serial.flush();
        if (response == "OK")
        {
            break;
        }
        yield();
    }

    return;
}

void Utility::waitMessage(String message)
{
    char buffer[100];
    String response;
    while (true)
    {
        while (!Serial.available())
        {
            Serial.read();
            yield();
        }

        delay(100);

        Serial.readBytes(buffer, 100);

        for (int i = 0; i < 100; i++)
        {
            if (buffer[i] == '-')
            {
                while (i < 100 && buffer[i + 1] != '-')
                {
                    response += buffer[i + 1];
                }
            }
        }

        response.trim();
        // Serial.println(response);
        // Serial.flush();
        if (response == message)
        {
            Serial.println("OK");
            Serial.flush();
            break;
        }
        yield();
    }

    return;
}
