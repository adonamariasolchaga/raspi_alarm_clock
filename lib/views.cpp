#include "views.hpp"

void HomeView::render(LCDdisplay& lcd, TimeUtils& clock) {
    lcd.clear();
    char buf[10];
    datetime_t current = clock.now();
    snprintf(
        buf, sizeof(buf), "%02d:%02d:%02d",
        current.hour, current.min, current.sec
    );

    lcd.goto_pos(4, 0);
    lcd.print(buf);
    lcd.goto_pos(15, 1);
    lcd.print("M");
}

void MenuView::render(LCDdisplay& lcd, TimeUtils& clock) {
    lcd.clear();
    lcd.goto_pos(0, 0);
    lcd.print("1:Set Alarm");
    lcd.goto_pos(0, 1);
    lcd.print("2:List Alarms");
}
