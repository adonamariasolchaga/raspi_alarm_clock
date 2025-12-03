#include "time_utils.hpp"
#include "hardware/rtc.h"


TimeUtils::TimeUtils(int year, int month, int day, int hour, int min, int sec) {
    current_time.year = year;
    current_time.month = month;
    current_time.day = day;
    current_time.hour = hour;
    current_time.min = min;
    current_time.sec = sec;
    current_time.dotw = 0; // Set day of the week if needed

    rtc_init();
    rtc_set_datetime(&current_time);
}

datetime_t TimeUtils::now() {
    datetime_t t;
    rtc_get_datetime(&t);
    return t;
}

void TimeUtils::addAlarm(int hour, int minute) {
    alarms.push_back({hour, minute});
}

const std::vector<Alarm>& TimeUtils::getAlarms() const {
    return alarms;
}

void TimeUtils::removeAlarm(int index) {
    if (index >= 0 && index < (int)alarms.size()) {
        alarms.erase(alarms.begin() + index);
    }
}

bool TimeUtils::checkAlarmTrigger(const datetime_t& t) const {
    for (const auto& a : alarms) {
        if (t.hour == a.hour && t.min == a.minute) {
            return true;
        }
    }
    return false;
}
