#pragma once
#include "pico/util/datetime.h"
#include <vector>

struct Alarm {
    int hour;
    int minute;
    bool triggered = false;
    bool paused = false;
};

class TimeUtils {
public:
    TimeUtils(int year, int month, int day, int hour, int min, int sec);
    datetime_t now();

    void addAlarm(int hour, int minute);
    const std::vector<Alarm>& getAlarms() const;
    bool checkAlarmTrigger(const datetime_t& now);
    void removeAlarm(int index);
    void pauseTriggeredAlarms();

private:
    datetime_t initial_time;
    std::vector<Alarm> alarms;
};
