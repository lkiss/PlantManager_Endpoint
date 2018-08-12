#include <Wire.h>
#include <RtcDS3231.h>
#include "LowPower.h"

#include "./devices/devices.h"
#include "./services/services.h"
#include "./constants.h"

int errorLedPin = LED_BUILTIN;
int numberOfWifiRestAttempts = 0;
int DHT11Pin = 11;
int waterPumpPin = 9;
int waterSensorEchoPin = 12;
int waterSensorTriggerPin = 13;
int sensorPowerPin = 10;
int soilMoisturePin = A0;
int wakeUpPin = INT0;
int wifiWakeupPin = 4;
int wifiReadyPin = 3;

bool isWifiPowered = false;
bool isSensorPowered = false;

int baudRate = 9600;

TemperatureSensor temperatureSensor(&DHT11Pin);
SoilMoistureSensor soilMoistureSensor(soilMoisturePin);
WaterPump waterPump(&waterPumpPin);
WaterLevelSensor waterLevelSensor(waterSensorTriggerPin, waterSensorEchoPin);
WaterTank waterTank(PRISM, 16, 6.5, 12);
RealTimeClock realTimeClock;

JsonService jsonService;
ConfigService configService;
DataService dataService(configService, jsonService);
SensorService sensorService(waterTank, waterLevelSensor, waterPump, soilMoistureSensor, temperatureSensor);

void wakeUp() {}

void wakeUpWifi()
{
  digitalWrite(wifiWakeupPin, LOW);
  delay(100);
  digitalWrite(wifiWakeupPin, HIGH);
}

void toggleSensors()
{
  if (!isSensorPowered)
  {
    digitalWrite(sensorPowerPin, HIGH);
    isSensorPowered = true;
  }
  else
  {
    digitalWrite(sensorPowerPin, LOW);
    isSensorPowered = false;
  }
}

void setup()
{
  pinMode(errorLedPin, OUTPUT);
  pinMode(sensorPowerPin, OUTPUT);
  pinMode(wifiWakeupPin, OUTPUT);
  pinMode(wakeUpPin, INPUT);
  pinMode(wifiReadyPin, INPUT);

  realTimeClock.InitializeRtc();
  wakeUpWifi();
  delay(3000);
}

void loop()
{
  if (digitalRead(wifiReadyPin) == HIGH)
  {
    Serial.begin(baudRate);
    Serial.setTimeout(10000);

    Serial.println(SENSOR_ID);

    while (!Serial.available())
    {
    }

    String configString = Serial.readStringUntil('\n');

    configService.setConfigurationJson(configString);
    Configuration config = configService.getConfiguration();

    sensorService.updateSensorsParamaters(config);

    toggleSensors();
    SensorReading reading = sensorService.getSensorReadings();
    toggleSensors();

    String jsonResult = jsonService.convertSensorReadingsToJson(reading);
    Serial.println(jsonResult);
    Serial.flush();

    sensorService.water(reading);

    attachInterrupt(wakeUpPin, wakeUp, FALLING);
    realTimeClock.UpdateTimer(config.MeasuringIntervalInMinutes);
    // Serial.print("MeasuringIntervalInMinutes: ");
    // Serial.println(config.MeasuringIntervalInMinutes);
    // Serial.print("SoilMoistureThreshold: ");
    // Serial.println(config.SoilMoistureThreshold);
    // Serial.print("WateringTimeInSeconds: ");
    // Serial.println(config.WateringTimeInSeconds);
    // Serial.print("minimumWaterThresholdPercentage: ");
    // Serial.println(config.MinimumWaterThresholdPercentage);
    Serial.end();
    numberOfWifiRestAttempts = 0;

    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    detachInterrupt(wakeUpPin);

    wakeUpWifi();
    delay(3000);
  }
  else
  {
    if (numberOfWifiRestAttempts == configService.getConfiguration().MaximumNumberOfWifiRestAttempts)
    {
      for (int i = 0; i < 10; i++)
      {
        digitalWrite(errorLedPin, HIGH);
        delay(1000);
        digitalWrite(errorLedPin, LOW);
        delay(1000);
      }
      digitalWrite(errorLedPin, HIGH);
      LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    }
    numberOfWifiRestAttempts++;
    wakeUpWifi();
    delay(5000);
  }
}