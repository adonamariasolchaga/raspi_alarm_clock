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
    if (cursorIndex < firstVisibleIndex)  firstVisibleIndex = cursorIndex;
    if (cursorIndex >= firstVisibleIndex + noLines) firstVisibleIndex = cursorIndex - noLines + 1;

    if (buttons.is_pressedC()) {
        if (options[cursorIndex].action) options[cursorIndex].action();
    }

    if (moved) updateFlag = true;
}

void CreateAlarmView::render(LCDdisplay& lcd, TimeUtils& clock) {
    lcd.clear();

    // There are 4 items: 0=Hour, 1=Minute, 2=Save, 3=Cancel
    const int totalItems = 4;

    // Determine which two items are visible
    int firstVisible = 0;

    if (cursorIndex > 1)
        firstVisible = cursorIndex - 1;

    char line[16];

    for (int row = 0; row < 2; ++row) {
        int item = firstVisible + row;
        if (item >= totalItems) break;

        lcd.goto_pos(0, row);
        lcd.print(item == cursorIndex ? ">" : " ");

        switch(item) {
            case 0:
                snprintf(line, sizeof(line), "Hour: %02d", hour);
                lcd.print(line);
                break;

            case 1:
                snprintf(line, sizeof(line), "Minute: %02d", minute);
                lcd.print(line);
                break;

            case 2:
                lcd.print("[Save]");
                break;

            case 3:
                lcd.print("[Cancel]");
                break;
        }
    }

    // ---- SCROLL ARROWS ----
    
    // Up arrow (^)
    if (firstVisible > 0) {
        lcd.goto_pos(15, 0);
        lcd.print("^");
    }

    // Down arrow (v)
    if (firstVisible + 2 < totalItems) {
        lcd.goto_pos(15, 1);
        lcd.print("v");
    }
}

void CreateAlarmView::handleInput(ButtonManager& buttons) {
    bool changed = false;
    
    if (buttons.is_pressedU() && cursorIndex > 0){
        cursorIndex--;
        changed = true;
    }

    if (buttons.is_pressedD() && cursorIndex < 3){
        cursorIndex++;
        changed = true;
    }

    if (cursorIndex == 0) {          // Hour
        if (buttons.is_pressedR()) {hour = (hour + 1) % 24; changed = true;}
        if (buttons.is_pressedL()) {hour = (hour + 23) % 24; changed = true;}
    }

    if (cursorIndex == 1) {          // Minute
        if (buttons.is_pressedR()) {minute = (minute + 1) % 60; changed = true;}
        if (buttons.is_pressedL()) {minute = (minute + 59) % 60; changed = true;}
    }

    if (buttons.is_pressedC()) {
        if (cursorIndex == 2) {
            // SAVE
            saveRequested = true;
        }
        if (cursorIndex == 3) {
            // CANCEL
            cancelRequested = true;

        }
    }

    if (changed) updateFlag = true;
}
