#include <RtcDS3231.h>
#include <Wire.h>
#include "LowPower.h"

#include "./devices/devices.h"
#include "./services/services.h"
#include "./constants.h"

int errorLedPin = LED_BUILTIN;
int numberOfWifiRestAttempts = 0;
int DHT11Pin = 11;
int waterPumpPin00 = 9;
int waterPumpPin01 = 8;
int waterSensorEchoPin = 12;
int waterSensorTriggerPin = 13;
int sensorPowerPin = 10;
int soilMoisturePin00 = A0;
int soilMoisturePin01 = A1;
int wakeUpPin = INT0;
int wifiWakeupPin = 4;
int wifiReadyPin = 3;
int currentEndpointId = 0;

bool isWifiPowered = false;
bool isSensorPowered = false;

int baudRate = 9600;

RtcDS3231<TwoWire> Rtc(Wire);
TemperatureSensor temperatureSensor(&DHT11Pin);
SoilMoistureSensor soilMoistureSensor00(soilMoisturePin00);
SoilMoistureSensor soilMoistureSensor01(soilMoisturePin01);
WaterPump waterPump00(&waterPumpPin00);
WaterPump waterPump01(&waterPumpPin01);
WaterLevelSensor waterLevelSensor(waterSensorTriggerPin, waterSensorEchoPin);
WaterTank waterTank(PRISM, 16, 6.5, 12);

WaterPump waterPumps[2]{waterPump00, waterPump01};
SoilMoistureSensor soilMoistureSensors[2]{soilMoistureSensor00, soilMoisturePin01};

JsonService jsonService;
ConfigService configService;
DataService dataService(configService, jsonService);
SensorService sensorService(waterTank, waterLevelSensor, waterPumps, soilMoistureSensors, temperatureSensor);

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

void updateTimer(int measuringIntervalInMinutes)
{
  RtcDateTime now = Rtc.GetDateTime();

  RtcDateTime wakeUpTime = now + (measuringIntervalInMinutes * 60);

  DS3231AlarmOne wakeTimer(
      wakeUpTime.Day(),
      wakeUpTime.Hour(),
      wakeUpTime.Minute(),
      wakeUpTime.Second(),
      DS3231AlarmOneControl_HoursMinutesSecondsMatch);
  Rtc.SetAlarmOne(wakeTimer);
  Rtc.LatchAlarmsTriggeredFlags();
}

void initializeRtc()
{
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

  if (!Rtc.IsDateTimeValid())
  {
    Rtc.SetDateTime(compiled);
  }

  if (!Rtc.GetIsRunning())
  {
    Rtc.SetIsRunning(true);
  }

  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled)
  {
    Rtc.SetDateTime(compiled);
  }

  Rtc.Enable32kHzPin(false);
  Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeAlarmBoth);
}

void setup()
{
  pinMode(errorLedPin, OUTPUT);
  pinMode(sensorPowerPin, OUTPUT);
  pinMode(wifiWakeupPin, OUTPUT);
  pinMode(wakeUpPin, INPUT);
  pinMode(wifiReadyPin, INPUT);
  // Serial.begin(baudRate);
  // Serial.println("Initialize RTC");
  initializeRtc();
  wakeUpWifi();

  // Serial.println("Begin processing");
  // Serial.end();
}

void loop()
{
  if (digitalRead(wifiReadyPin) == HIGH && currentEndpointId < NUMBER_OF_DEVICES)
  {
    Serial.begin(baudRate);
    Serial.setTimeout(10000);

    Serial.print(SENSOR_ID);
    Serial.print(" ");
    Serial.println(currentEndpointId);

    while (!Serial.available())
    {
      yield();
    }

    String configString = Serial.readStringUntil('\n');
    Configuration config = jsonService.convertJsonToConfig(configString);
    configService.addWakeUpDelay(config.MeasuringIntervalInMinutes, currentEndpointId);

    toggleSensors();

    sensorService.updateSensorParamaters(config, currentEndpointId);
    SensorReading reading = sensorService.getSensorReadings(currentEndpointId);

    toggleSensors();

    jsonService.printSensorReadingJson(reading);

    sensorService.water(reading, currentEndpointId);

    currentEndpointId++;
    if (currentEndpointId != NUMBER_OF_DEVICES)
    {
      delay(3000);
      wakeUpWifi();
      delay(3000);
    }
    Serial.end();
  }
  else if (currentEndpointId == NUMBER_OF_DEVICES)
  {
    currentEndpointId = 0;
    attachInterrupt(wakeUpPin, wakeUp, FALLING);
    int nextWakeUpDelay = configService.getNextWakeUpDelay();
    updateTimer(nextWakeUpDelay);

    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    detachInterrupt(wakeUpPin);

    wakeUpWifi();
    delay(3000);
  }
}