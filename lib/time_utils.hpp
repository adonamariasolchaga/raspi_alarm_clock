#pragma once
#include "pico/util/datetime.h"

class TimeUtils {
public:
    static void init();
    static datetime_t now();
};
