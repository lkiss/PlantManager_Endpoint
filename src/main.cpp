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
int wakeUpPin = INT1;
int wifiReadyPin = 9;

TemperatureSensor temperatureSensor(&DHT11Pin);
SoilMoistureSensor soilMoistureSensor(soilMoisturePin);
WaterPump waterPump(&waterPumpPin);
WaterLevelSensor waterLevelSensor(waterSensorTriggerPin, waterSensorEchoPin);
WaterTank waterTank(PRISM, 16, 6.5, 12);

JsonService jsonService;
ConfigService configService;
DataService dataService(configService, jsonService);
SensorService sensorService(waterTank, waterLevelSensor, waterPump, soilMoistureSensor, temperatureSensor);

void wakeUp() {}

void updateDevices(){
  Configuration config = configService.getConfiguration();

  waterPump.updateWateringTime(config.WateringTime);
  soilMoistureSensor.updateTresholdValues(config.SoilMoistureThreshold);
}

void setup()
{
  Serial.begin(9600);
  Serial.setTimeout(10000);
  pinMode(sensorPowerPin, OUTPUT);
  pinMode(wakeUpPin, INPUT);
  pinMode(wifiReadyPin, INPUT);
}

void loop()
{
  if (digitalRead(wifiReadyPin) == HIGH)
  {
    Serial.println(SENSOR_ID);
    Serial.flush();

    while (!Serial.available())
    {
      delay(100);
    }

    String configString = Serial.readStringUntil('\n');
    configService.setConfigurationJson(configString);

    updateDevices();

    digitalWrite(sensorPowerPin, HIGH);

    SensorReading reading = sensorService.getSensorReadings();
    sensorService.water(reading);

    digitalWrite(sensorPowerPin, LOW);

    Serial.println(jsonService.convertSensorReadingsToJson(reading, configService.getConfiguration()));
    Serial.end();

    attachInterrupt(wakeUpPin, wakeUp, LOW);

    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    Serial.begin(9600);
    detachInterrupt(wakeUpPin);
  }
}