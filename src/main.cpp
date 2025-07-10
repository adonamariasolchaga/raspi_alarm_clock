#include "lcd_display.hpp"
#include "lcd_app.hpp"

int main() {
    LCDdisplay lcd(2, 3, 4, 5, 14, 15, 16, 2);
    lcd.init();

    LCDApp app(lcd);
    app.run();

    return 0;
}

