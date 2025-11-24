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
