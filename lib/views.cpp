#include "views.hpp"
#include "button_manager.hpp"


// ----------------- HomeView -----------------
void HomeView::render(LCDdisplay& lcd, TimeUtils& clock) {
    lcd.clear();
    datetime_t current = clock.now();
    char buf[10];
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d", current.hour, current.min, current.sec);

    lcd.goto_pos(4,0);
    lcd.print(buf);

    // Botón "M" para acceder al menú
    lcd.goto_pos(15, 1);
    lcd.print("M");
    // Cursor delante de "M"
    lcd.goto_pos(14, 1);
    lcd.print(">");
}

void HomeView::handleInput(ButtonManager& buttons) {
    if (buttons.is_pressedC()) {
        // Acceso al menú (se puede cambiar la vista desde AlarmApp)
    }
}

// ----------------- ScrollableMenuView -----------------
void ScrollableMenuView::render(LCDdisplay& lcd, TimeUtils& clock) {
    lcd.clear();
    for (int i = 0; i < noLines; ++i) {
        int optIndex = firstVisibleIndex + i;
        if (optIndex >= options.size()) break;
        lcd.goto_pos(0, i);
        if (optIndex == cursorIndex) lcd.print(">"); else lcd.print(" ");
        lcd.print(options[optIndex].label);
    }

    // Indicadores de scroll
    if (firstVisibleIndex > 0) {
        lcd.goto_pos(15, 0);
        lcd.print("^");
    }
    if (firstVisibleIndex + noLines < options.size()) {
        lcd.goto_pos(15, noLines - 1);
        lcd.print("v");
    }
}

void ScrollableMenuView::handleInput(ButtonManager& buttons) {
    bool moved = false;
    if (buttons.is_pressedU() && cursorIndex > 0) {
        cursorIndex--;
        moved = true;
    }
    if (buttons.is_pressedD() && cursorIndex < options.size() - 1) {
        cursorIndex++;
        moved = true;
    }

    // Ajustar scroll
    if (cursorIndex < firstVisibleIndex) firstVisibleIndex = cursorIndex;
    if (cursorIndex >= firstVisibleIndex + noLines) firstVisibleIndex = cursorIndex - noLines + 1;

    if (buttons.is_pressedC()) {
        if (options[cursorIndex].action) options[cursorIndex].action();
        moved = true;
    }

    if (moved) render(*lcdPtr, *(new TimeUtils(2025,10,9,12,0,0)));
}
