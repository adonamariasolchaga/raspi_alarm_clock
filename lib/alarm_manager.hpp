#pragma once
#include "lcd_display.hpp"
#include "time_utils.hpp"
#include "views.hpp"
#include <map>
#include <memory>

// Enum for different views/screens
enum class View {
    Home,
    Menu,
    SetAlarm,
    AlarmList,
};

class AlarmApp {
public:
    AlarmApp(LCDdisplay& display, TimeUtils& clock);
    void run();
    void setView(View v);

private:
    LCDdisplay& lcd;
    View currentView;
    TimeUtils& clock;
    std::map<View, std::unique_ptr<IView>> views;
    void render();
};
