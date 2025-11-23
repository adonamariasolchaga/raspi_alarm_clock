#pragma once
#include "pico/stdlib.h"

class ButtonManager {
public:
    ButtonManager(uint button_up_pin, uint button_left_pin, uint button_right_pin, uint button_bottom_pin, uint button_center_pin);
    void init();
    bool is_pressedU();
    bool is_pressedD();
    bool is_pressedL();
    bool is_pressedR();
    bool is_pressedC();

private:
    uint up_pin;
    uint left_pin;
    uint right_pin;
    uint bottom_pin;
    uint pin_ok;
};
