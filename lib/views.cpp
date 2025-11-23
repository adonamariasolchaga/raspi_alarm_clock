#include "views.hpp"
#include "button_manager.hpp"

View IView::handleInput(ButtonManager& buttons) {
    return View::Home;
}

void HomeView::render(LCDdisplay& lcd, TimeUtils& clock) {
    lcd.clear();

    char buf[10];
    datetime_t current = clock.now();
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d",
             current.hour, current.min, current.sec);

    lcd.goto_pos(4, 0);
    lcd.print(buf);

    lcd.goto_pos(15, 1);
    lcd.print("M");
}
View HomeView::handleInput(ButtonManager& buttons) {
    if (buttons.is_pressedU())
        return View::Menu;
    return View::Home;
}

void MenuView::render(LCDdisplay& lcd, TimeUtils& clock) {
    lcd.clear();

    lcd.goto_pos(0, 0);
    lcd.print(selected == 0 ? "> Set Alarm" : "  Set Alarm");

    lcd.goto_pos(0, 1);
    lcd.print(selected == 1 ? "> List Alarms" : "  List Alarms");
}

View MenuView::handleInput(ButtonManager& buttons) {
    if (buttons.is_pressedU()) {
        selected = (selected - 1 + 2) % 2;
    }

    if (buttons.is_pressedD()) {
        selected = (selected + 1) % 2;
    }

    if (buttons.is_pressedL()) {
        return View::Home; // volver al home
    }

    if (buttons.is_pressedC()) {
        if (selected == 0) return View::CreateAlarm;
        if (selected == 1) return View::ListAlarms;
    }

    return View::Menu;
}
