#pragma once
#include "lcd_display.hpp"
#include "button_manager.hpp"
#include "time_utils.hpp"
#include "views.hpp"
#include <map>
#include <memory>


class AlarmApp {
public:
    AlarmApp(LCDdisplay& display, TimeUtils& clock, ButtonManager& buttons);
    void run();
    void setView(View v);

private:
    LCDdisplay& lcd;
    View currentView;
    TimeUtils& clock;
    ButtonManager& buttons;
    std::map<View, std::unique_ptr<IView>> views;
    void render();
    std::unique_ptr<HomeView> homeView;
    std::unique_ptr<ScrollableMenuView> menuView;
    std::unique_ptr<CreateAlarmView> createAlarmView;
    std::unique_ptr<ListAlarmsView> listAlarmsView;
    std::unique_ptr<DeleteAlarmView> deleteAlarmView;
    std::unique_ptr<AlarmTriggeredView> alarmTriggeredView;
};
