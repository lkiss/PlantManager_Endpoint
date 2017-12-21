#include "LowPower.h"
#include "./devices/devices.h"
#include "./services/services.h"
#include "./constants.h"

int DHT11Pin = 5;
int waterPumpPin = 6;
int waterSensorEchoPin = 7;
int waterSensorTriggerPin = 8;
int sensorPowerPin = 4;
int soilMoisturePin = 14;
int wakeUpPin = 1;

Dht11Sensor dht11Sensor(&DHT11Pin);
SoilMoistureSensor soilMoistureSensor(soilMoisturePin);
WaterPump waterPump(&waterPumpPin);
WaterLevelSensor waterLevelSensor(waterSensorTriggerPin, waterSensorEchoPin);
WaterTank waterTank(PRISM, 16, 6.5, 12);

JsonService jsonService;
ConfigService configService;
DataService dataService(configService, jsonService);
SensorService sensorService(waterTank, waterLevelSensor, waterPump, soilMoistureSensor, dht11Sensor);

void wakeUp(){}

void setup()
{
  pinMode(sensorPowerPin, OUTPUT);
  pinMode(wakeUpPin, INPUT);
}

void loop()
{
  digitalWrite(sensorPowerPin, HIGH);

  SensorReading reading = sensorService.getSensorReadings();
  sensorService.water(reading);

  digitalWrite(sensorPowerPin, LOW);

  Serial.begin(9600);

  Serial.println(jsonService.convertSensorReadingsToJson(reading, configService.getConfiguration()));
  Serial.end();

  attachInterrupt(wakeUpPin, wakeUp, LOW);

  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
  detachInterrupt(wakeUpPin);
}