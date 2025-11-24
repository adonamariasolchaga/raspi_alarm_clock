#include "alarm_manager.hpp"
#include "pico/stdlib.h"


AlarmApp::AlarmApp(LCDdisplay& display, TimeUtils& clock, ButtonManager& buttons) : lcd(display), currentView(View::Home), clock(clock), buttons(buttons) {
    homeView = std::make_unique<HomeView>(&lcd);
    std::vector<MenuOption> menuOpts = {
        {"Set Alarm", [](){ /* lógica set alarm */ }},
        {"List Alarms", [](){ /* lógica list alarm */ }},
        {"Home", [this](){ setView(View::Home); }}
    };
    menuView = std::make_unique<ScrollableMenuView>(&lcd, menuOpts, 2);
}

void AlarmApp::run() {
    while(true) {
        switch(currentView) {
            case View::Home:
                homeView->render(lcd, clock);
                homeView->handleInput(buttons);
                if (buttons.is_pressedC()) currentView = View::Menu;
                break;
            case View::Menu:
                menuView->render(lcd, clock);
                menuView->handleInput(buttons);
                break;
        }
        sleep_ms(100); // retardo pequeño para debouncing
    }
}

void AlarmApp::setView(View v) {
    currentView = v;
}

void AlarmApp::render() {
    views[currentView]->render(lcd, clock);
}
