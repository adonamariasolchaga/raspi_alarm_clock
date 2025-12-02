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
    ListAlarms
};

class IView {
public:
    virtual ~IView() = default;
    virtual void render(LCDdisplay& lcd, TimeUtils& clock) = 0;
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
public:
    HomeView(LCDdisplay* lcd) : lcdPtr(lcd) {}
    void render(LCDdisplay& lcd, TimeUtils& clock) override;
    void handleInput(ButtonManager& buttons) override;
};


// ----------------- ScrollableMenuView -----------------
struct MenuOption {
    const char* label;
    std::function<void()> action;
};

class ScrollableMenuView : public IView {
private:
    LCDdisplay* lcdPtr;
    std::vector<MenuOption> options;
    int cursorIndex = 0;
    int firstVisibleIndex = 0;
    int noLines;
public:
    ScrollableMenuView(LCDdisplay* lcd, std::vector<MenuOption> opts, int lines)
        : lcdPtr(lcd), options(opts), noLines(lines) {}
    void render(LCDdisplay& lcd, TimeUtils& clock) override;
    void handleInput(ButtonManager& buttons) override;
    int getCursorIndex() const { return cursorIndex; }
    void setCursorIndex(int idx) { cursorIndex = idx; }
};


// ----------------- CreateAlarmView -----------------
class CreateAlarmView : public IView {
private:
    LCDdisplay* lcdPtr;

    int cursorIndex = 0;  // 0=hour, 1=minute, 2=save, 3=cancel
    int hour = 7;
    int minute = 30;

    bool saveRequested = false;
    bool cancelRequested = false;

public:
    CreateAlarmView(LCDdisplay* lcd)
        : lcdPtr(lcd) {}

    void render(LCDdisplay& lcd, TimeUtils& clock) override;
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

    std::vector<Alarm> alarms;

    int cursorIndex = 0;        // cursor in the virtual list (alarms + Back)
    int firstVisibleIndex = 0;  // top index shown on screen

    bool updateRequested = true;
    bool backRequested = false;

public:
    ListAlarmsView(LCDdisplay* lcd) : lcdPtr(lcd) {}

    void setAlarms(const std::vector<Alarm>& list) {
        alarms = list;
    }

    bool needsUpdate() const { return updateRequested; }
    void resetUpdateFlag() { updateRequested = false; }

    bool isBackRequested() const { return backRequested; }
    void resetBackRequest() { backRequested = false; }

    void render(LCDdisplay& lcd, TimeUtils& clock) override;
    void handleInput(ButtonManager& buttons) override;
};
