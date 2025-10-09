#pragma once
#include "lcd_display.hpp"
#include "time_utils.hpp"
#include <cstdio>

class IView {
public:
    virtual ~IView() = default;
    virtual void render(LCDdisplay& lcd, TimeUtils& clock) = 0;
};

class HomeView : public IView {
public:
    void render(LCDdisplay& lcd, TimeUtils& clock) override;
};

class MenuView : public IView {
public:
    void render(LCDdisplay& lcd, TimeUtils& clock) override;
};
