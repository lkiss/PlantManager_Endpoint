#include "./timer.h"
#include <RtcDS3231.h>

Timer::Timer()
{
    this->timer = new RtcDS3231<TwoWire>(Wire);

    this->timer->Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

    if (!this->timer->IsDateTimeValid())
    {
        Serial.println("RTC lost confidence in the DateTime!");
        this->timer->SetDateTime(compiled);
    }

    if (!this->timer->GetIsRunning())
    {
        Serial.println("RTC was not actively running, starting now");
        this->timer->SetIsRunning(true);
    }

    RtcDateTime now = this->timer->GetDateTime();
    if (now < compiled)
    {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        this->timer->SetDateTime(compiled);
    }

    this->timer->Enable32kHzPin(false);
    this->timer->SetSquareWavePin(DS3231SquareWavePin_ModeAlarmBoth);
}

void Timer::setTimeOffsetInMinutes(int timeOffsetInMinutes)
{
    RtcDateTime now = this->timer->GetDateTime();
    RtcDateTime wakeUpTime = now + (timeOffsetInMinutes * 60);
    DS3231AlarmOne wakeTimer(
        wakeUpTime.Day(),
        wakeUpTime.Hour(),
        wakeUpTime.Minute(),
        wakeUpTime.Second(),
        DS3231AlarmOneControl_HoursMinutesSecondsMatch);
    this->timer->SetAlarmOne(wakeTimer);
    this->timer->LatchAlarmsTriggeredFlags();
}