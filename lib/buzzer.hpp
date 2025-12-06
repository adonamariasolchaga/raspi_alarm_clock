#pragma once
#include "pico/stdlib.h"

class Buzzer {
public:
    Buzzer(uint pin);

    // Active buzzer: simply turn ON/OFF
    void on();
    void off();

private:
    uint pin;
};
