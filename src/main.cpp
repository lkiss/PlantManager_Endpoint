#include <Wire.h>
#include <RtcDS3231.h>
#include "LowPower.h"

#include "./devices/devices.h"
#include "./services/services.h"
#include "./constants.h"
#include "./utility/utility.h"

int statusLedPin = LED_BUILTIN;
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

int waterSensorPowerPin = 7;

bool isWifiPowered = false;
bool isSensorPowered = false;

int baudRate = 9600;

RtcDS3231<TwoWire> Rtc(Wire);
TemperatureSensor temperatureSensor(&DHT11Pin);
SoilMoistureSensor soilMoistureSensor00(soilMoisturePin00);
SoilMoistureSensor soilMoistureSensor01(soilMoisturePin01);
WaterPump waterPump00(waterPumpPin00, statusLedPin);
WaterPump waterPump01(waterPumpPin01, statusLedPin);
WaterLevelSensor waterLevelSensor(waterSensorTriggerPin, waterSensorEchoPin);
WaterTank waterTank(CYLINDER, 0, 0, 10, 10);

WaterPump waterPumps[2]{waterPump00, waterPump01};
SoilMoistureSensor soilMoistureSensors[2]{soilMoistureSensor00, soilMoisturePin01};

JsonService jsonService;
ConfigService configService;
DataService dataService(configService, jsonService);
SensorService sensorService(waterTank, waterLevelSensor, waterPumps, soilMoistureSensors, temperatureSensor);

Utility utilities;
Configuration config;

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
    digitalWrite(waterSensorPowerPin, HIGH);
    isSensorPowered = true;
  }
  else
  {
    digitalWrite(sensorPowerPin, LOW);
    digitalWrite(waterSensorPowerPin, LOW);
    isSensorPowered = false;
  }
}

void updateTimer(int measuringIntervalInMinutes)
{
  RtcDateTime now = Rtc.GetDateTime();

  RtcDateTime wakeUpTime = now + (measuringIntervalInMinutes * 60);

  DS3231AlarmOne wakeTimer01(
      wakeUpTime.Day(),
      wakeUpTime.Hour(),
      wakeUpTime.Minute(),
      wakeUpTime.Second(),
      DS3231AlarmOneControl_HoursMinutesSecondsMatch);
  Rtc.SetAlarmOne(wakeTimer01);

  Rtc.LatchAlarmsTriggeredFlags();
  // Serial.println("Timer updated");
}

void initializeRtc()
{
  Rtc.Begin();

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  
  // Serial.println("After RtcDateTime");

  if (!Rtc.IsDateTimeValid())
  {
    Rtc.SetDateTime(compiled);
  }

  // Serial.println("After IsDateTimeValid");

  if (!Rtc.GetIsRunning())
  {
    Rtc.SetIsRunning(true);
  }

  // Serial.println("After GetIsRunning");

  RtcDateTime now = Rtc.GetDateTime();
  // Serial.println("After Rtc.GetDateTime()");
  if (now < compiled)
  {
    Rtc.SetDateTime(compiled);
  }

  // Serial.println("Before Enable32kHzPin");
  Rtc.Enable32kHzPin(false);
  Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeAlarmBoth);
}

void setup()
{
  pinMode(statusLedPin, OUTPUT);
  pinMode(sensorPowerPin, OUTPUT);
  pinMode(waterSensorPowerPin, OUTPUT);
  pinMode(wifiWakeupPin, OUTPUT);
  pinMode(wakeUpPin, INPUT);
  pinMode(wifiReadyPin, INPUT);
  Serial.begin(baudRate);
  // Serial.println("Initialize RTC");
  initializeRtc();
  wakeUpWifi();
}

void loop()
{
  if (digitalRead(wifiReadyPin) == HIGH && currentEndpointId < NUMBER_OF_DEVICES)
  {
    utilities.oscillatePin(statusLedPin, 500, 1);
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
    config = jsonService.convertJsonToConfig(configString);
    // configService.addWakeUpDelay(config.MeasuringIntervalInMinutes, currentEndpointId);

    // Serial.println("Before toggleSensors");

    toggleSensors();

    sensorService.updateSensorParamaters(config, currentEndpointId);
    SensorReading reading = sensorService.getSensorReadings(currentEndpointId);

    toggleSensors();

    // Serial.println("After toggleSensors");

    jsonService.printSensorReadingJson(reading);

    utilities.oscillatePin(statusLedPin, 500, 2);

    sensorService.water(reading, currentEndpointId);

    currentEndpointId++;
    if (currentEndpointId < NUMBER_OF_DEVICES)
    {
      delay(3000);
      yield();
      wakeUpWifi();
      delay(3000);
    }
    Serial.end();
  }
  else if (currentEndpointId == NUMBER_OF_DEVICES)
  {
    // Serial.println("MeasurinIntervalInMinutes");
    // Serial.println(config.MeasuringIntervalInMinutes);

    updateTimer(config.MeasuringIntervalInMinutes);

    utilities.oscillatePin(statusLedPin, 500, 3);

    attachInterrupt(wakeUpPin, wakeUp, FALLING);
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    detachInterrupt(wakeUpPin);

    currentEndpointId = 0;

    utilities.oscillatePin(statusLedPin, 500, 3);

    wakeUpWifi();
    delay(3000);
  }
}