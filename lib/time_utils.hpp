#pragma once
#include "pico/util/datetime.h"
#include <vector>

struct Alarm {
    int hour;
    int minute;
};

class TimeUtils {
public:
    TimeUtils(int year, int month, int day, int hour, int min, int sec);
    datetime_t now();

    void addAlarm(int hour, int minute);
    const std::vector<Alarm>& getAlarms() const;
    bool checkAlarmTrigger(const datetime_t& t) const;

private:
    datetime_t current_time;
    std::vector<Alarm> alarms;
};