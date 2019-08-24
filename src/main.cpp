#include <Wire.h>
#include <RtcDS3231.h>
#include <CD74HC4067.h>
#include "LowPower.h"

#include "./devices/devices.h"
#include "./services/services.h"
#include "./constants.h"
#include "./utility/utility.h"

int statusLedPin = LED_BUILTIN;
int numberOfWifiRestAttempts = 0;
int DHT11Pin = 10;
int waterPumpPin00 = 9;
int waterPumpPin01 = 8;
int waterSensorEchoPin = 8;
int waterSensorTriggerPin = 9;
int sensorPowerPin = 5;
int muxCommonPin = A0;
int wakeUpPin = INT0;
int wifiWakeupPin = 4;
int wifiReadyPin = 3;
int endpointIndex = 0;
int waterSensorPowerPin = 7;
int baudRate = 9600;

int s0 = 6;
int s1 = 13;
int s2 = 12;
int s3 = 11;

bool isWifiPowered = false;
bool isSensorPowered = false;

CD74HC4067 mux(s0, s1, s2, s3);
RtcDS3231<TwoWire> Rtc(Wire);
TemperatureSensor temperatureSensor(&DHT11Pin);

SoilMoistureSensor soilMoistureSensor00(0);
SoilMoistureSensor soilMoistureSensor01(1);
SoilMoistureSensor soilMoistureSensor02(2);
SoilMoistureSensor soilMoistureSensor03(3);
SoilMoistureSensor soilMoistureSensor04(4);
SoilMoistureSensor soilMoistureSensor05(5);
SoilMoistureSensor soilMoistureSensor06(6);
SoilMoistureSensor soilMoistureSensor07(7);

WaterPump waterPump00(0, statusLedPin);
WaterPump waterPump01(1, statusLedPin);
WaterPump waterPump02(2, statusLedPin);
WaterPump waterPump03(3, statusLedPin);
WaterPump waterPump04(4, statusLedPin);
WaterPump waterPump05(5, statusLedPin);
WaterPump waterPump06(6, statusLedPin);
WaterPump waterPump07(7, statusLedPin);

WaterLevelSensor waterLevelSensor(waterSensorTriggerPin, waterSensorEchoPin);
WaterTank waterTank(CYLINDER, 0, 0, 10, 10);

WaterPump waterPumps[NUMBER_OF_DEVICES]{
  waterPump00, 
  waterPump01,
  waterPump02,
  waterPump03,
  waterPump04,
  waterPump05,
  waterPump06,
  waterPump07
  };

SoilMoistureSensor soilMoistureSensors[NUMBER_OF_DEVICES]{
  soilMoistureSensor00,
  soilMoistureSensor01,
  soilMoistureSensor02,
  soilMoistureSensor03,
  soilMoistureSensor04,
  soilMoistureSensor05,
  soilMoistureSensor06,
  soilMoistureSensor07,
  };

JsonService jsonService;
ConfigService configService;
DataService dataService(configService, jsonService);
SensorService sensorService(waterTank, waterLevelSensor, waterPumps, soilMoistureSensors, temperatureSensor, mux);

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
  if (digitalRead(wifiReadyPin) == HIGH && endpointIndex < NUMBER_OF_DEVICES)
  {
    utilities.oscillatePin(statusLedPin, 500, 1);
    Serial.begin(baudRate);
    Serial.setTimeout(10000);

    Serial.print(SENSOR_ID);
    Serial.print(" ");
    Serial.println(endpointIndex);

    while (!Serial.available())
    {
      yield();
    }

    String configString = Serial.readStringUntil('\n');
    config = jsonService.convertJsonToConfig(configString);
    // configService.addWakeUpDelay(config.MeasuringIntervalInMinutes, endpointIndex);

    // Serial.println("Before toggleSensors");

    toggleSensors();

    sensorService.updateSensorParamaters(config, endpointIndex);
    pinMode(muxCommonPin, INPUT);
    delay(50);

    SensorReading reading = sensorService.getSensorReadings(endpointIndex);

    toggleSensors();

    // Serial.println("After toggleSensors");

    jsonService.printSensorReadingJson(reading);
    utilities.oscillatePin(statusLedPin, 500, 2);

    pinMode(muxCommonPin, OUTPUT);
    delay(50);

    sensorService.water(reading, endpointIndex + 8);

    endpointIndex++;
    if (endpointIndex < NUMBER_OF_DEVICES)
    {
      delay(3000);
      yield();
      wakeUpWifi();
      delay(3000);
    }
    Serial.end();
  }
  else if (endpointIndex == NUMBER_OF_DEVICES)
  {
    // Serial.begin(baudRate);
    // Serial.println("NUMBER_OF_DEVICES: ");
    // Serial.println(NUMBER_OF_DEVICES);
    // Serial.println("MeasurinIntervalInMinutes");
    // Serial.println(config.MeasuringIntervalInMinutes);
    // Serial.end();

    updateTimer(config.MeasuringIntervalInMinutes);

    utilities.oscillatePin(statusLedPin, 500, 3);

    attachInterrupt(wakeUpPin, wakeUp, FALLING);
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    detachInterrupt(wakeUpPin);

    endpointIndex = 0;

    utilities.oscillatePin(statusLedPin, 500, 3);

    wakeUpWifi();

    // Serial.begin(baudRate);
    // Serial.println("Wakeup");
    // Serial.end();

    delay(3000);
  }
}