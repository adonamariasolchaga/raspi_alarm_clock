#include "alarm_manager.hpp"
#include "button_manager.hpp"
#include "lcd_display.hpp"
#include "time_utils.hpp"
#include "initial_setup_view.hpp"

int main() {
    // Initialize onboard LED for debugging
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);

    // Create the display object
    LCDdisplay lcd(2, 3, 4, 5, 14, 15, 16, 2);
    lcd.init();

    // Create the button manager object
    ButtonManager buttons(9, 10, 11, 12, 13);
    buttons.init();

    // Initial setup screen
    InitialSetupView setup;

    while (!setup.update(buttons)) {
        setup.render(lcd);
        sleep_ms(100);
    }

    int year, month, day, hour, minute;
    setup.getResult(year, month, day, hour, minute);

    TimeUtils myClock(year, month, day, hour, minute, 0);

    // Create and run the alarm application
    AlarmApp app(lcd, myClock, buttons);
    app.run();

    return 0;
}
