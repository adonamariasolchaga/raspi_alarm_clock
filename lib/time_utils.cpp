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
