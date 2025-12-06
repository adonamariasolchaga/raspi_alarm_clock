#pragma once
#include "lcd_display.hpp"
#include "time_utils.hpp"
#include <cstdio>
#include <functional>
#include <vector>

// Forward declaration
class ButtonManager;

enum class View {
    Home,
    Menu,
    CreateAlarm,
    ListAlarms,
    DeleteAlarms,
    AlarmTriggered,
};

class IView {
public:
    virtual ~IView() = default;
    virtual void render(LCDdisplay& lcd) = 0;
    virtual void handleInput(ButtonManager& buttons) = 0;


    bool needsUpdate() const { return updateFlag; }
    void resetUpdateFlag() { updateFlag = false; }

protected:
    bool updateFlag = true;
};

// ----------------- HomeView -----------------
class HomeView : public IView {
private:
    LCDdisplay* lcdPtr;
    TimeUtils* clock;
    datetime_t lastRenderedTime;
    bool secondLineDrawn = false;
    char lastPrinted[10] = "00:00:00";

public:
    HomeView(LCDdisplay* lcd, TimeUtils* clock)
        : lcdPtr(lcd), clock(clock), lastRenderedTime(clock->now()) {}
    bool secondElapsed();
    void render(LCDdisplay& lcd) override;
    void handleInput(ButtonManager& buttons) override;
    void reset() { secondLineDrawn = false; }
};


// ----------------- ScrollableMenuView -----------------
struct MenuOption {
    const char* label;
    std::function<void()> action;
};

class ScrollableMenuView : public IView {
private:
    LCDdisplay* lcdPtr;
    TimeUtils* clock;
    std::vector<MenuOption> options;
    int cursorIndex = 0;
    int firstVisibleIndex = 0;
    int noLines;
public:
    ScrollableMenuView(LCDdisplay* lcd,  TimeUtils* clock, std::vector<MenuOption> opts, int lines)
        : lcdPtr(lcd), clock(clock), options(opts), noLines(lines) {}
    void render(LCDdisplay& lcd) override;
    void handleInput(ButtonManager& buttons) override;
    int getCursorIndex() const { return cursorIndex; }
    void setCursorIndex(int idx) { cursorIndex = idx; }
};


// ----------------- CreateAlarmView -----------------
class CreateAlarmView : public IView {
private:
    LCDdisplay* lcdPtr;
    TimeUtils* clock;

    int cursorIndex = 0;  // 0=hour, 1=minute, 2=save, 3=cancel
    int hour = 7;
    int minute = 30;

    bool saveRequested = false;
    bool cancelRequested = false;

public:
    CreateAlarmView(LCDdisplay* lcd, TimeUtils* clock)
        : lcdPtr(lcd), clock(clock) {}

    void render(LCDdisplay& lcd) override;
    void handleInput(ButtonManager& buttons) override;

    // Getters
    int getHour() const { return hour; }
    int getMinute() const { return minute; }

    // Event flags for AlarmApp
    bool isSaveRequested() const { return saveRequested; }
    bool isCancelRequested() const { return cancelRequested; }

    // Reset flags when entering view
    void resetEvents() { saveRequested = false; cancelRequested = false; }
};

// ----------------- ListAlarmsView -----------------
class ListAlarmsView : public IView {
private:
    LCDdisplay* lcdPtr;
    TimeUtils* clock;

    int cursorIndex = 0;        // cursor in the virtual list (alarms + Back)
    int firstVisibleIndex = 0;  // top index shown on screen

    bool updateRequested = true;
    bool backRequested = false;

public:
    ListAlarmsView(LCDdisplay* lcd, TimeUtils* clock) : lcdPtr(lcd), clock(clock) {}

    bool needsUpdate() const { return updateRequested; }
    void resetUpdateFlag() { updateRequested = false; }

    bool isBackRequested() const { return backRequested; }
    void resetBackRequest() { backRequested = false; }

    void render(LCDdisplay& lcd) override;
    void handleInput(ButtonManager& buttons) override;
};

// ----------------- DeleteAlarmView -----------------
class DeleteAlarmView : public IView {
private:
    LCDdisplay* lcdPtr;
    TimeUtils* clock;

    int cursorIndex = 0;
    int firstVisibleIndex = 0;

    bool updateRequested = true;
    bool backRequested = false;

    bool confirming = false;   // true = showing YES/NO window
    int confirmChoice = 0;     // 0 = Yes, 1 = No

public:
    DeleteAlarmView(LCDdisplay* lcd, TimeUtils* clock) : lcdPtr(lcd), clock(clock) {}

    bool needsUpdate() const { return updateRequested; }
    void resetUpdateFlag() { updateRequested = false; }

    bool isBackRequested() const { return backRequested; }
    void resetBackRequest() { backRequested = false; }

    void render(LCDdisplay& lcd) override;
    void handleInput(ButtonManager& buttons) override;
};

// ----------------- AlarmTriggeredView -----------------
class AlarmTriggeredView : public IView {
private:
    LCDdisplay* lcdPtr;
    TimeUtils* clock;
    bool changed = true;
    bool visible = true;        // for blinking
    bool firstRender = true;
    datetime_t triggerTime;

public:
    AlarmTriggeredView(LCDdisplay* lcd, TimeUtils* clock) : lcdPtr(lcd), clock(clock) {}

    void setTriggerTime(const datetime_t& t) { triggerTime = t; }

    void render(LCDdisplay& lcd) override;
    void handleInput(ButtonManager& buttons) override;

    bool needsUpdate() const { return changed; }
    void resetUpdateFlag() { changed = false; }
    void reset();

};
