#include "views.hpp"
#include "button_manager.hpp"


// ----------------- HomeView -----------------
void HomeView::render(LCDdisplay& lcd)
{
    datetime_t current = clock->now();
    char buf[10];
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d", current.hour, current.min, current.sec);

    // Clear all and draw secon line only on forst render
    if (!secondLineDrawn)
    {
        lcd.clear();
        lcd.goto_pos(11, 1);
        lcd.print(">Menu");
        secondLineDrawn = true;
        lcd.goto_pos(4, 0);
        lcd.print(buf);
    }

    // Update only FIRST LINE
    // Write ONLY digits that changed
    // Time starts at column 4 on line 0
    const int baseCol = 4;

    for (int i = 0; i < 8; i++)
    {
        if (buf[i] != lastPrinted[i])
        {
            lcd.goto_pos(baseCol + i, 0);
            char oneChar[2] = { buf[i], '\0' };
            lcd.print(oneChar);
        }
    }

    // Store for next comparison
    std::copy(buf, buf + 9, lastPrinted);
    lastPrinted[8] = '\0';
}

void HomeView::handleInput(ButtonManager& buttons) {
    bool moved = false;
    if (buttons.is_pressedC()) {
        moved = true;
    }
    if (moved) updateFlag = true;
}

bool HomeView::secondElapsed() {
    datetime_t current = clock->now();
    if (current.sec != lastRenderedTime.sec) {
        lastRenderedTime = current;
        return true;
    }
    return false;
}

// ----------------- ScrollableMenuView -----------------
void ScrollableMenuView::render(LCDdisplay& lcd) {
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

void CreateAlarmView::render(LCDdisplay& lcd) {
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

// ----------------- ListAlarmsView -----------------
void ListAlarmsView::render(LCDdisplay& lcd) {
    lcd.clear();
    std::vector<Alarm> alarms = clock->getAlarms();
    // Case 1: NO ALARMS
    if (alarms.empty()) {
        lcd.goto_pos(0, 0);
        lcd.print("NO ALARMS");

        lcd.goto_pos(0, 1);
        lcd.print(">Back");
        return;
    }

    // Number of entries: alarms + back option
    int totalEntries = alarms.size() + 1;

    // Show two visible lines
    for (int i = 0; i < 2; ++i) {
        int idx = firstVisibleIndex + i;
        if (idx >= totalEntries) break;

        lcd.goto_pos(0, i);

        // Pointer
        lcd.print(idx == cursorIndex ? ">" : " ");

        if (idx < alarms.size()) {
            // Show alarm time
            char buf[16];
            snprintf(buf, sizeof(buf), "%02d:%02d", 
                     alarms[idx].hour, alarms[idx].minute);
            lcd.print(buf);
        } else {
            // Last entry → Back
            lcd.print("Back");
        }
    }

    // Scroll indicators
    if (firstVisibleIndex > 0) {
        lcd.goto_pos(15, 0);
        lcd.print("^");
    }

    if (firstVisibleIndex + 2 < totalEntries) {
        lcd.goto_pos(15, 1);
        lcd.print("v");
    }
}


void ListAlarmsView::handleInput(ButtonManager& buttons) {
    updateRequested = false;

    std::vector<Alarm> alarms = clock->getAlarms();

    // Case 1: NO ALARMS
    if (alarms.empty()) {
        if (buttons.is_pressedC()) {
            backRequested = true;
            cursorIndex = 0;
            return;
        }
        return; // no scrolling possible
    }

    // Normal case: alarms exist
    int totalEntries = alarms.size() + 1; // alarms + Back

    bool changed = false;

    if (buttons.is_pressedU() && cursorIndex > 0) {
        cursorIndex--;
        changed = true;
    }
    if (buttons.is_pressedD() && cursorIndex < totalEntries - 1) {
        cursorIndex++;
        changed = true;
    }

    // Scroll adjustment
    if (cursorIndex < firstVisibleIndex) {
        firstVisibleIndex = cursorIndex;
        changed = true;
    }
    if (cursorIndex >= firstVisibleIndex + 2) {
        firstVisibleIndex = cursorIndex - 1;
        changed = true;
    }

    // OK pressed
    if (buttons.is_pressedC()) {
        if (cursorIndex == totalEntries - 1) {
            backRequested = true; // user selected Back
            cursorIndex = 0;
            
        }
    }

    if (changed) updateRequested = true;
}

// ----------------- DeleteAlarmView -----------------
void DeleteAlarmView::render(LCDdisplay& lcd) {
    lcd.clear();
    std::vector<Alarm> alarms = clock->getAlarms();

    // --- Confirmation window ---
    if (confirming) {
        lcd.goto_pos(0, 0);
        lcd.print("Delete?");
        lcd.goto_pos(0, 1);
        lcd.print(confirmChoice == 0 ? ">Yes" : " Yes");
        lcd.print(confirmChoice == 1 ? " >No" : "  No");
        return;
    }

    // --- Normal list mode ---
    if (alarms.empty()) {
        lcd.goto_pos(0, 0);
        lcd.print("NO ALARMS");

        lcd.goto_pos(0, 1);
        lcd.print(cursorIndex == 0 ? ">" : " ");
        lcd.print("Back");
        return;
    }

    int totalEntries = alarms.size() + 1;

    for (int i = 0; i < 2; ++i) {
        int idx = firstVisibleIndex + i;
        if (idx >= totalEntries) break;

        lcd.goto_pos(0, i);
        lcd.print(idx == cursorIndex ? ">" : " ");

        if (idx < alarms.size()) {
            char buf[16];
            snprintf(buf, sizeof(buf), "%02d:%02d",
                     alarms[idx].hour, alarms[idx].minute);
            lcd.print(buf);
        } else {
            lcd.print("Back");
        }
    }

    if (firstVisibleIndex > 0) {
        lcd.goto_pos(15, 0);
        lcd.print("^");
    }

    if (firstVisibleIndex + 2 < totalEntries) {
        lcd.goto_pos(15, 1);
        lcd.print("v");
    }
}


void DeleteAlarmView::handleInput(ButtonManager& buttons) {
    updateRequested = false;
    std::vector<Alarm> alarms = clock->getAlarms();

    // -------- CONFIRMATION MODE --------
    if (confirming) {
        bool changed = false;

        if (buttons.is_pressedL() && confirmChoice > 0) {
            confirmChoice = 0;
            changed = true;
        }
        if (buttons.is_pressedR() && confirmChoice < 1) {
            confirmChoice = 1;
            changed = true;
        }

        if (buttons.is_pressedC()) {
            if (confirmChoice == 0) {
                // --- YES: delete alarm ---
                // Delete from global alarm list
                clock->removeAlarm(cursorIndex);

                // Fix cursor if needed
                if (cursorIndex >= (int)alarms.size())
                    cursorIndex = alarms.empty() ? 0 : alarms.size() - 1;

                firstVisibleIndex = 0;
            }

            // Exit confirmation window regardless of choice
            confirming = false;
            changed = true;
        }

        if (changed) updateRequested = true;
        return;
    }

    // -------- NORMAL LIST MODE --------
    if (alarms.empty()) {
        if (buttons.is_pressedC()) {
            backRequested = true;
        }
        return;
    }

    int totalEntries = alarms.size() + 1;
    bool changed = false;

    if (buttons.is_pressedU() && cursorIndex > 0) {
        cursorIndex--;
        changed = true;
    }
    if (buttons.is_pressedD() && cursorIndex < totalEntries - 1) {
        cursorIndex++;
        changed = true;
    }

    if (cursorIndex < firstVisibleIndex) {
        firstVisibleIndex = cursorIndex;
        changed = true;
    }
    if (cursorIndex >= firstVisibleIndex + 2) {
        firstVisibleIndex = cursorIndex - 1;
        changed = true;
    }

    if (buttons.is_pressedC()) {
        if (cursorIndex == totalEntries - 1) {
            backRequested = true;  // Back selected
        } else {
            confirming = true;
            confirmChoice = 0; // default to YES
        }
        changed = true;
    }

    if (changed) updateRequested = true;
}

// ----------------- AlarmTriggeredView -----------------
void AlarmTriggeredView::render(LCDdisplay& lcd)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "%02d:%02d", triggerTime.hour, triggerTime.min);

    // FIRST TIME entering this view → clear whole screen once
    if (firstRender)
    {
        lcd.clear();

        // Print static second line
        lcd.goto_pos(0, 1);
        lcd.print("PRESS ANY BTN");

        firstRender = false;   // Next render will not clear
    }

    // --- Update ONLY the first line (blink) ---
    lcd.goto_pos(0, 0);
    lcd.print("                ");  // Clear first line (16 spaces)

    if (visible)
    {
        lcd.goto_pos(0, 0);
        lcd.print("ALARM !! - ");
        lcd.print(buf);
    }

    // Toggle blinking state
    visible = !visible;
}

void AlarmTriggeredView::handleInput(ButtonManager& buttons)
{
    // If ANY button pressed → exit
    if (buttons.is_pressedU() ||
        buttons.is_pressedD() ||
        buttons.is_pressedL() ||
        buttons.is_pressedR() ||
        buttons.is_pressedC())
    {
        // App will handle leaving the view
        changed = true;
    }
}

void AlarmTriggeredView::reset()
{
    firstRender = true;
    visible = true;
}
