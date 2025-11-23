#include "alarm_manager.hpp"
#include "pico/stdlib.h"


AlarmApp::AlarmApp(LCDdisplay& display, TimeUtils& clock, ButtonManager& buttons) : lcd(display), currentView(View::Home), clock(clock), buttons(buttons) {
    views[View::Home] = std::make_unique<HomeView>();
    views[View::Menu] = std::make_unique<MenuView>();
    views[View::CreateAlarm] = std::make_unique<MenuView>(); // temporal
    views[View::ListAlarms]  = std::make_unique<MenuView>(); // temporal
}

void AlarmApp::run() {
    while (true) {
        render();

        View next = views[currentView]->handleInput(buttons);

        if (next != currentView) {
            setView(next);
        }

        sleep_ms(120); // debounce
    }
}

void AlarmApp::setView(View v) {
    currentView = v;
}

void AlarmApp::render() {
    views[currentView]->render(lcd, clock);
}
