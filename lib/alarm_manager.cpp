#include "alarm_manager.hpp"
#include "pico/stdlib.h"


AlarmApp::AlarmApp(LCDdisplay& display, TimeUtils& clock, ButtonManager& buttons) : lcd(display), currentView(View::Home), clock(clock), buttons(buttons) {
    homeView = std::make_unique<HomeView>(&lcd);
    createAlarmView = std::make_unique<CreateAlarmView>(&lcd);

    std::vector<MenuOption> menuOpts = {
        {"Set Alarm", [this](){ setView(View::CreateAlarm); }},
        {"List Alarms", [](){ /* lógica list alarm */ }},
        {"Home", [this](){ setView(View::Home); }}
    };
    menuView = std::make_unique<ScrollableMenuView>(&lcd, menuOpts, 2);
}

void AlarmApp::run() {
    while(true) {

        if (clock.checkAlarmTrigger(clock.now())) {
            // TODO: trigger buzzer / LED / Alarm screen
        }
        switch(currentView) {
            case View::Home:
                homeView->handleInput(buttons);

                if (homeView->needsUpdate()) { // TODO: modify to update screen when time changes
                    homeView->render(lcd, clock);
                    homeView->resetUpdateFlag();
                }

                if (buttons.is_pressedC()) {
                    setView(View::Menu);
                }
                break;
            case View::Menu:
                menuView->handleInput(buttons);

                if (menuView->needsUpdate()) {
                    menuView->render(lcd, clock);
                    menuView->resetUpdateFlag();
                }

                break;
            case View::CreateAlarm:
                createAlarmView->handleInput(buttons);

                if (createAlarmView->needsUpdate()) {
                    createAlarmView->render(lcd, clock);
                    createAlarmView->resetUpdateFlag();
                }

                if (createAlarmView->isSaveRequested()) {
                    clock.addAlarm(
                        createAlarmView->getHour(),
                        createAlarmView->getMinute()
                    );
                    createAlarmView->resetEvents();
                    setView(View::Menu);
                }

                if (createAlarmView->isCancelRequested()) {
                    createAlarmView->resetEvents();
                    setView(View::Menu);
                }

                break;

        }
        sleep_ms(10); // retardo pequeño para debouncing
    }
}

void AlarmApp::setView(View v) {
    currentView = v;
    switch(currentView) {
        case View::Home:
            homeView->resetUpdateFlag();
            homeView->render(lcd, clock);
            break;
        case View::Menu:
            menuView->resetUpdateFlag();
            menuView->render(lcd, clock);
            break;
        case View::CreateAlarm:
            createAlarmView->resetUpdateFlag();
            createAlarmView->render(lcd, clock);
            break;
        default:
            break;
    }
}

void AlarmApp::render() {
    views[currentView]->render(lcd, clock);
}
