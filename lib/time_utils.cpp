#include "time_utils.hpp"
#include "hardware/rtc.h"

void TimeUtils::init() {
    rtc_init();

    // Check if RTC contains valid data
    datetime_t t;
    bool ok = rtc_get_datetime(&t);
    if (!ok || t.year < 2000 || t.year > 2100) {
        // RTC not initialized, set default time
        datetime_t default_time = {
            .year  = 2025,
            .month = 10,
            .day   = 8,
            .dotw  = 3,  // 0 = Sunday, 3 = Wednesday, etc.
            .hour  = 12,
            .min   = 0,
            .sec   = 0
        };
        rtc_set_datetime(&default_time);
    }
}

datetime_t TimeUtils::now() {
    datetime_t t;
    rtc_get_datetime(&t);
    return t;
}