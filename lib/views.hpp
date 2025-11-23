#pragma once
#include "lcd_display.hpp"
#include "time_utils.hpp"
#include <cstdio>

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
    virtual View handleInput(ButtonManager& buttons);
};

class HomeView : public IView {
public:
    void render(LCDdisplay& lcd, TimeUtils& clock) override;
    View handleInput(ButtonManager& buttons) override;
};

class MenuView : public IView {
private:
    int selected = 0; // 0 = Set Alarm, 1 = List Alarms
public:
    void render(LCDdisplay& lcd, TimeUtils& clock) override;
    View handleInput(ButtonManager& buttons) override;
};
