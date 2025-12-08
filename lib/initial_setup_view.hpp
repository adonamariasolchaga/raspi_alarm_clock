#pragma once
#include "lcd_display.hpp"
#include "button_manager.hpp"
#include "time_utils.hpp"
#include <cstdio>

class InitialSetupView {
public:
    InitialSetupView();

    // returns true when user finishes setup
    bool update(ButtonManager &buttons);

    enum PointerPos {
        PTR_VALUE,
        PTR_OK
    };
    
    // draw on screen
    void render(LCDdisplay &lcd);

    // when done, retrieve chosen time
    void getResult(int &year, int &month, int &day, int &hour, int &minute);

private:
    enum Field { YEAR, MONTH, DAY, HOUR, MINUTE};
    Field field;
    PointerPos pointer = PTR_VALUE;

    int year, month, day, hour, minute;
    bool finished;
};
