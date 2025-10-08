#include "lcd_app.hpp"
#include "time_utils.hpp"
#include <cstdio>

LCDApp::LCDApp(LCDdisplay& display) : lcd(display), currentView(View::Home) {}

void LCDApp::run() {
    int counter = 0;
    TimeUtils::init();
    char buf[10];

    while (true) {
        // Update time reference
        datetime_t current = TimeUtils::now();
        snprintf(buf, sizeof(buf), "%02d:%02d:%02d",
            // current.day, current.month, current.year,
            current.hour, current.min, current.sec);

        // Update view
        render(buf);
        // sleep_ms(100); // Wait 1 second (replace with your timing function)
        // counter++;
        // Cycle through views every second for demonstration
        // if (counter % 4 == 0) setView(View::Home);
        // else if (counter % 4 == 1) setView(View::Menu);
        // else if (counter % 4 == 2) setView(View::SetAlarm);
        // else if (counter % 4 == 3) setView(View::AlarmList);
    }
}

void LCDApp::setView(View v) {
    currentView = v;
}

void LCDApp::render(const char* buf) {
    lcd.clear();
    switch (currentView) {
        case View::Home:
            lcd.goto_pos(4, 0); // First line, first position
            lcd.print(buf);
            lcd.goto_pos(0, 1); // Second line, first position
            lcd.print("Alarm: ON");
            break;
        case View::Menu:
            lcd.goto_pos(0, 0);
            lcd.print("1:Set Alarm");
            lcd.goto_pos(0, 1);
            lcd.print("2:List Alarms");
            break;
        // Add more complex views as needed
    }
}
