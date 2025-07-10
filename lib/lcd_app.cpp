#include "lcd_app.hpp"

LCDApp::LCDApp(LCDdisplay& display) : lcd(display), currentView(View::Home) {}

void LCDApp::run() {
    int counter = 0;
    while (true) {
        render();
        sleep_ms(1000); // Wait 1 second (replace with your timing function)
        counter++;
        // Cycle through views every second for demonstration
        if (counter % 4 == 0) setView(View::Home);
        else if (counter % 4 == 1) setView(View::Menu);
        else if (counter % 4 == 2) setView(View::SetAlarm);
        else if (counter % 4 == 3) setView(View::AlarmList);
    }
}

void LCDApp::setView(View v) {
    currentView = v;
}

void LCDApp::render() {
    lcd.clear();
    switch (currentView) {
        case View::Home:
            lcd.goto_pos(0, 0); // First line, first position
            lcd.print("Time: 12:34");
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
