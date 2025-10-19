#include "alarm_manager.hpp"
#include "button_manager.hpp"
#include "lcd_display.hpp"
#include "time_utils.hpp"

int main() {
    // Create the display object
    LCDdisplay lcd(2, 3, 4, 5, 14, 15, 16, 2);
    lcd.init();

    // Create the time utility object
    TimeUtils myClock(2025, 10, 9, 12, 0, 0);

    // Create the button manager object
    ButtonManager buttons(10, 11, 12, 13, 16);
    buttons.init();

    // Create and run the alarm application
    AlarmApp app(lcd, myClock, buttons);
    app.run();

    return 0;
}
