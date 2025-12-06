#include "time_utils.hpp"
#include "hardware/rtc.h"


TimeUtils::TimeUtils(int year, int month, int day, int hour, int min, int sec) {
    initial_time.year = year;
    initial_time.month = month;
    initial_time.day = day;
    initial_time.hour = hour;
    initial_time.min = min;
    initial_time.sec = sec;
    initial_time.dotw = 0; // Set day of the week if needed

    rtc_init();
    rtc_set_datetime(&initial_time);
}

datetime_t TimeUtils::now() {
    datetime_t t;
    rtc_get_datetime(&t);
    return t;
}

void TimeUtils::addAlarm(int hour, int minute) {
    alarms.push_back({hour, minute, false});
}

const std::vector<Alarm>& TimeUtils::getAlarms() const {
    return alarms;
}

void TimeUtils::removeAlarm(int index) {
    if (index >= 0 && index < (int)alarms.size()) {
        alarms.erase(alarms.begin() + index);
    }
}

bool TimeUtils::checkAlarmTrigger(const datetime_t& now)
{
    for (auto& alarm : alarms)
    {
        if (!alarm.triggered &&
            !alarm.paused &&
            now.hour == alarm.hour &&
            now.min  == alarm.minute)
        {
            alarm.triggered = true;  // <-- Mark as triggered
            return true;             // Fire alarm ONCE
        }
        
        // Reset pause and trigger at a new minute
        if (now.min != alarm.minute || now.hour != alarm.hour)
        {
            alarm.paused = false;
            alarm.triggered = false;
        }
    }
    return false;
}

void TimeUtils::pauseTriggeredAlarms()
{
    for (auto& alarm : alarms)
    {
        if (alarm.triggered)
            alarm.paused = true;
    }
}
