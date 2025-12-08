#include "initial_setup_view.hpp"

InitialSetupView::InitialSetupView()
    : field(YEAR),
      year(2025),
      month(1),
      day(1),
      hour(12),
      minute(0),
      finished(false)
{}

bool InitialSetupView::update(ButtonManager &buttons)
{
    if (finished) return true;

    bool changed = false;

    // 1) LEFT/RIGHT => move pointer between VALUE and OK
    if (buttons.is_pressedL()) {
        if (pointer == PTR_OK) {
            pointer = PTR_VALUE;
            changed = true;
        }
    }
    if (buttons.is_pressedR()) {
        if (pointer == PTR_VALUE) {
            pointer = PTR_OK;
            changed = true;
        }
    }

    // 2) UP/DOWN => modify current value ONLY when pointer is on VALUE
    if (pointer == PTR_VALUE) {
        if (buttons.is_pressedU()) {
            switch (field) {
                case YEAR:   year++; changed = true; break;
                case MONTH:  if (month < 12) { month++; changed = true; } break;
                case DAY:    if (day < 31)   { day++; changed = true; } break;
                case HOUR:   if (hour < 23)  { hour++; changed = true; } break;
                case MINUTE: if (minute < 59){ minute++; changed = true; } break;
                default: break;
            }
        }

        if (buttons.is_pressedD()) {
            switch (field) {
                case YEAR:   if (year > 2000) { year--; changed = true; } break;
                case MONTH:  if (month > 1)   { month--; changed = true; } break;
                case DAY:    if (day > 1)     { day--; changed = true; } break;
                case HOUR:   if (hour > 0)    { hour--; changed = true; } break;
                case MINUTE: if (minute > 0)  { minute--; changed = true; } break;
                default: break;
            }
        }
    }

    // 3) OK pressed:
    if (buttons.is_pressedC()) {
        if (pointer == PTR_OK) {

            if (field == MINUTE) {
                // Last editable field → immediately finish setup
                finished = true;
                return true;
            }

            // Otherwise advance to next field
            field = Field(field + 1);

            // Reset pointer when moving to a new field
            pointer = PTR_VALUE;
        }
    }

    return finished;
}

void InitialSetupView::render(LCDdisplay &lcd)
{
    static bool headerPrinted = false;
    static Field lastField = MINUTE;
    static int lastY=-1, lastMo=-1, lastD=-1, lastH=-1, lastMi=-1;
    static PointerPos lastPointer = PTR_VALUE;

    // ---- FIRST LINE: printed once ----
    if (!headerPrinted) {
        lcd.goto_pos(0, 0);
        lcd.print("Set Date/Time");
        headerPrinted = true;
    }

    // ---- Build value portion ----
    char value[17];
    switch (field) {
        case YEAR:
            snprintf(value, sizeof(value), "Year:%04d", year);
            break;
        case MONTH:
            snprintf(value, sizeof(value), "Month:%02d", month);
            break;
        case DAY:
            snprintf(value, sizeof(value), "Day:%02d", day);
            break;
        case HOUR:
            snprintf(value, sizeof(value), "Hour:%02d", hour);
            break;
        case MINUTE:
            snprintf(value, sizeof(value), "Min:%02d", minute);
            break;
    }

    // ---- Build full second line ----
    char line[17];

    if (pointer == PTR_VALUE)
        snprintf(line, sizeof(line), ">%s^v  OK", value);
    else
        snprintf(line, sizeof(line), " %s^v >OK", value);

    // ---- Detect if we must update LCD ----
    bool changed =
       field != lastField ||
       pointer != lastPointer ||
       (field == YEAR   && year   != lastY)  ||
       (field == MONTH  && month  != lastMo) ||
       (field == DAY    && day    != lastD)  ||
       (field == HOUR   && hour   != lastH)  ||
       (field == MINUTE && minute != lastMi);

    if (changed) {
        lcd.goto_pos(0,1);
        lcd.print("                ");
        lcd.goto_pos(0,1);
        lcd.print(line);
    }

    // ---- save last printed state ----
    lastField = field;
    lastPointer = pointer;
    lastY = year;
    lastMo = month;
    lastD = day;
    lastH = hour;
    lastMi = minute;
}

void InitialSetupView::getResult(int &y, int &m, int &d, int &h, int &min)
{
    y = year;
    m = month;
    d = day;
    h = hour;
    min = minute;
}
