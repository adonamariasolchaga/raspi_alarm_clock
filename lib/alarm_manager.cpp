#include "alarm_manager.hpp"
#include "time_utils.hpp"
#include "pico/stdlib.h"


AlarmApp::AlarmApp(LCDdisplay& display, TimeUtils& clock, ButtonManager& buttons) : lcd(display), currentView(View::Home), clock(clock), buttons(buttons) {
    views[View::Home] = std::make_unique<HomeView>();
    views[View::Menu] = std::make_unique<MenuView>();
}

void AlarmApp::run() {
    while (true) {
        render();
        if (buttons.is_pressedU()) {
            setView(View::Home);
        } else {
            setView(View::Menu);
        }

        // Simulate view changes for demonstration purposes
        // counter++;
        // if (counter == 2) {
        //     setView(View::Menu);
        // } else if (counter == 4) {
        //     setView(View::Home);
        //     counter = 0;
        // }
        // sleep_ms(1000); // Wait for 1 second
    }
}

void AlarmApp::setView(View v) {
    currentView = v;
}

void AlarmApp::render() {
    views[currentView]->render(lcd, clock);
}
