#pragma once
#include "lcd_display.hpp"

// Enum for different views/screens
enum class View {
    Home,
    Menu,
    SetAlarm,
    AlarmList,
    // Add more as needed
};

class LCDApp {
public:
    LCDApp(LCDdisplay& display);
    void run();
    void setView(View v);

private:
    LCDdisplay& lcd;
    View currentView;
    void render(const char* buf);
}; 