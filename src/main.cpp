#include "lcd_display.hpp"
#include "alarm_manager.hpp"
#include "time_utils.hpp"

int main() {
    LCDdisplay lcd(2, 3, 4, 5, 14, 15, 16, 2);
    lcd.init();

    TimeUtils myClock(2025, 10, 9, 12, 0, 0);

    AlarmApp app(lcd, myClock);
    app.run();

    return 0;
}
