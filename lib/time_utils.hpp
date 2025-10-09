#pragma once
#include "pico/util/datetime.h"

class TimeUtils {
public:
    TimeUtils(int year, int month, int day, int hour, int min, int sec);
    datetime_t now();

private:
    datetime_t current_time;
};