#include <Wire.h>
#include <RtcDS3231.h>
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
int wifiPowerPin = 10;

TemperatureSensor temperatureSensor(&DHT11Pin);
SoilMoistureSensor soilMoistureSensor(soilMoisturePin);
WaterPump waterPump(&waterPumpPin);
WaterLevelSensor waterLevelSensor(waterSensorTriggerPin, waterSensorEchoPin);
WaterTank waterTank(PRISM, 16, 6.5, 12);
RtcDS3231<TwoWire> Rtc(Wire);

JsonService jsonService;
ConfigService configService;
DataService dataService(configService, jsonService);
SensorService sensorService(waterTank, waterLevelSensor, waterPump, soilMoistureSensor, temperatureSensor);

void wakeUp() {}

void initializeTimer(int measuringIntervalInMinutes)
{
  Rtc.Begin();

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

  RtcDateTime wakeUpTime = now + (measuringIntervalInMinutes * 60);

  int nowMinute = now.Minute();
  int futureMinute = wakeUpTime.Minute();

  DS3231AlarmOne wakeTimer(
      wakeUpTime.Day(),
      wakeUpTime.Hour(),
      wakeUpTime.Minute(),
      wakeUpTime.Second(),
      DS3231AlarmOneControl_HoursMinutesSecondsMatch);
  Rtc.SetAlarmOne(wakeTimer);
  Rtc.LatchAlarmsTriggeredFlags();
}

void updateDevices(Configuration config)
{

  waterPump.updateWateringTime(config.WateringTimeInSeconds);
  soilMoistureSensor.updateTresholdValues(config.SoilMoistureThreshold);
}

void setup()
{
  Serial.begin(9600);
  Serial.setTimeout(10000);
  pinMode(sensorPowerPin, OUTPUT);
  pinMode(wifiPowerPin, OUTPUT);
  pinMode(wakeUpPin, INPUT);
  pinMode(wifiReadyPin, INPUT);

  digitalWrite(wifiPowerPin, LOW);
}

void loop()
{
  digitalWrite(wifiPowerPin, HIGH);
  if (digitalRead(wifiReadyPin) == HIGH)
  {
    Serial.println(SENSOR_ID);

    while (!Serial.available())
    {
      delay(100);
    }

    String configString = Serial.readStringUntil('\n');
    configService.setConfigurationJson(configString);
    Configuration config = configService.getConfiguration();

    updateDevices(config);

    digitalWrite(sensorPowerPin, HIGH);

    SensorReading reading = sensorService.getSensorReadings();
    sensorService.water(reading);

    digitalWrite(sensorPowerPin, LOW);

    String jsonResult = jsonService.convertSensorReadingsToJson(reading);

    Serial.println(jsonResult);

    while (!Serial.available())
    {
      delay(100);
    }

    attachInterrupt(wakeUpPin, wakeUp, FALLING);
    digitalWrite(wifiPowerPin, LOW);
    initializeTimer(config.MeasuringIntervalInMinutes);
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    detachInterrupt(wakeUpPin);
  }
}