#include <Wire.h>
#include <RtcDS3231.h>
#include <CD74HC4067.h>
#include "LowPower.h"

#include "./devices/devices.h"
#include "./services/services.h"
#include "./constants.h"
#include "./utility/utility.h"

int statusLedPin = 7;
int numberOfWifiRestAttempts = 0;
int DHT11Pin = 10;
int waterSensorEchoPin = 9;
int waterSensorTriggerPin = 8;
int sensorPowerPin = 5;
int wakeUpPin = INT0;
int wifiWakeupPin = 4;
int wifiReadyPin = 3;
int endpointIndex = 0;
int baudRate = 9600;

int s0 = 6;
int s1 = 13;
int s2 = 12;
int s3 = 11;

bool isWifiPowered = false;
bool isSensorPowered = false;

String serialMessage = "";
char inChar = "";
int terminatorCharacterCount = 0;

bool isConfigSuccess = false;
bool isWifiReady = false;
bool isPayload = false;

CD74HC4067 mux(s0, s1, s2, s3);
RtcDS3231<TwoWire> Rtc(Wire);
TemperatureSensor temperatureSensor(&DHT11Pin);

SoilMoistureSensor soilMoistureSensor00(0);
SoilMoistureSensor soilMoistureSensor01(1);
// SoilMoistureSensor soilMoistureSensor02(2);
// SoilMoistureSensor soilMoistureSensor03(3);
// SoilMoistureSensor soilMoistureSensor04(4);
// SoilMoistureSensor soilMoistureSensor05(5);
// SoilMoistureSensor soilMoistureSensor06(6);
// SoilMoistureSensor soilMoistureSensor07(7);

WaterPump waterPump00(0, statusLedPin);
WaterPump waterPump01(1, statusLedPin);
// WaterPump waterPump02(2, statusLedPin);
// WaterPump waterPump03(3, statusLedPin);
// WaterPump waterPump04(4, statusLedPin);
// WaterPump waterPump05(5, statusLedPin);
// WaterPump waterPump06(6, statusLedPin);
// WaterPump waterPump07(7, statusLedPin);

WaterLevelSensor waterLevelSensor(waterSensorTriggerPin, waterSensorEchoPin);
WaterTank waterTank(CYLINDER, 0, 0, 10, 10);

WaterPump waterPumps[NUMBER_OF_DEVICES]{
    waterPump00,
    waterPump01};

SoilMoistureSensor soilMoistureSensors[NUMBER_OF_DEVICES]{
    soilMoistureSensor00,
    soilMoistureSensor01,
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
    isSensorPowered = true;
  }
  else
  {
    digitalWrite(sensorPowerPin, LOW);
    isSensorPowered = false;
  }
  delay(200);
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
  // pinMode(statusLedPin, OUTPUT);
  pinMode(sensorPowerPin, OUTPUT);
  pinMode(wifiWakeupPin, OUTPUT);
  pinMode(wakeUpPin, INPUT);
  pinMode(wifiReadyPin, INPUT);
  pinMode(MUX_COMMON_PIN, INPUT);
  Serial.begin(baudRate);
  Serial.setTimeout(5000);
  // Serial.println("Initialize RTC");
  initializeRtc();
  // Serial.println("Initialize RTC done");
  wakeUpWifi();
  Serial.flush();
}

void loop()
{
  if (isConfigSuccess && isPayload)
  {
    // String configString = Serial.readStringUntil('\n');
    config = jsonService.convertJsonToConfig(serialMessage);

    toggleSensors();

    sensorService.updateSensorParamaters(config, endpointIndex);
    SensorReading reading = sensorService.getSensorReadings(endpointIndex);

    toggleSensors();

    Serial.print("*READING_SUCCESS*");

    jsonService.printSensorReadingJson(reading);

    Serial.flush();

    sensorService.water(reading, endpointIndex);

    endpointIndex++;
    if (endpointIndex < NUMBER_OF_DEVICES)
    {
      delay(3000);
      yield();
      wakeUpWifi();
    }

    isConfigSuccess = false;
    isPayload = false;
    serialMessage = "";
  }

  if (isWifiReady && endpointIndex < NUMBER_OF_DEVICES)
  {
    Serial.print("*SENSOR_ID_SUCCESS*");
    Serial.print("*");
    Serial.print(DEVICE_ID);
    Serial.print(" ");
    Serial.print(endpointIndex);
    Serial.print("*");
    Serial.flush();

    isWifiReady = false;
  }

  if (endpointIndex == NUMBER_OF_DEVICES)
  {
    // Serial.begin(baudRate);
    // Serial.println("NUMBER_OF_DEVICES: ");
    // Serial.println(NUMBER_OF_DEVICES);
    // Serial.println("MeasurinIntervalInMinutes");
    // Serial.println(config.MeasuringIntervalInMinutes);
    // Serial.end();

    updateTimer(config.MeasuringIntervalInMinutes);

    attachInterrupt(wakeUpPin, wakeUp, FALLING);
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    detachInterrupt(wakeUpPin);

    endpointIndex = 0;

    wakeUpWifi();

    // Serial.begin(baudRate);
    Serial.println("Wakeup");
    // Serial.end();

    delay(3000);
  }
}

void serialEvent()
{
  while (Serial.available())
  {
    inChar = (char)Serial.read();

    // Serial.print(inChar);

    if (terminatorCharacterCount == 1)
    {
      if (inChar != '*')
      {
        serialMessage += inChar;
      }
    }

    // Serial.print(serialMessage);

    if (inChar == '*')
    {
      terminatorCharacterCount++;
    }

    if (terminatorCharacterCount == 2)
    {
      while (Serial.available())
      {
        Serial.read();
      }

      Serial.flush();
      if (serialMessage.length() != 0)
      {
        Serial.print(serialMessage);
        if (serialMessage == "CONFIG_SUCCESS")
        {
          isConfigSuccess = true;
          serialMessage = "";
        }
        else if (serialMessage == "WIFI_READY")
        {
          isWifiReady = true;
          serialMessage = "";
        }
        else
        {
          isPayload = true;
        }
      }
      terminatorCharacterCount = 0;
    }
  }
}