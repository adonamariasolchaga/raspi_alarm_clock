#include "alarm_manager.hpp"
#include "pico/stdlib.h"


AlarmApp::AlarmApp(LCDdisplay& display, TimeUtils& clock, ButtonManager& buttons) : lcd(display), currentView(View::Home), clock(clock), buttons(buttons) {
    homeView = std::make_unique<HomeView>(&lcd, &clock);
    createAlarmView = std::make_unique<CreateAlarmView>(&lcd, &clock);

    std::vector<MenuOption> menuOpts = {
        {"Create Alarm", [this](){ setView(View::CreateAlarm); }},
        {"List Alarms", [this](){ setView(View::ListAlarms); }},
        {"Delete Alarms", [this](){ setView(View::DeleteAlarms); }},
        {"Home", [this](){ setView(View::Home); }}
    };
    menuView = std::make_unique<ScrollableMenuView>(&lcd, &clock, menuOpts, 2);
    listAlarmsView = std::make_unique<ListAlarmsView>(&lcd, &clock);
    deleteAlarmView = std::make_unique<DeleteAlarmView>(&lcd, &clock);
    alarmTriggeredView = std::make_unique<AlarmTriggeredView>(&lcd, &clock);
}

void AlarmApp::run() {
    // Init with home view
    setView(View::Home);

    while(true) {

        // Check if any alarm is triggered
        datetime_t now = clock.now();
        if (clock.checkAlarmTrigger(now) && currentView != View::AlarmTriggered)
        {
            // Light alarm indicator if needed
            gpio_put(25, 1);

            alarmTriggeredView->setTriggerTime(now);
            setView(View::AlarmTriggered);
        }

        // Handle view interaction logic
        switch(currentView) {
            case View::Home:
                homeView->handleInput(buttons);
                if (homeView->secondElapsed()) {
                    homeView->render(lcd);
                }
                if (homeView->needsUpdate()) {
                    setView(View::Menu);
                    homeView->resetUpdateFlag();
                }

                break;
            case View::Menu:
                menuView->handleInput(buttons);

                if (menuView->needsUpdate()) {
                    menuView->render(lcd);
                    menuView->resetUpdateFlag();
                }

                break;
            case View::CreateAlarm:
                createAlarmView->handleInput(buttons);

                if (createAlarmView->needsUpdate()) {
                    createAlarmView->render(lcd);
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
            case View::ListAlarms:
                listAlarmsView->handleInput(buttons);

                if (listAlarmsView->needsUpdate()) {
                    listAlarmsView->render(lcd);
                    listAlarmsView->resetUpdateFlag();
                }

                if (listAlarmsView->isBackRequested()) {
                    listAlarmsView->resetBackRequest();
                    setView(View::Menu);
                }
                break;
            case View::DeleteAlarms:
                deleteAlarmView->handleInput(buttons);

                if (deleteAlarmView->needsUpdate()) {
                    deleteAlarmView->render(lcd);
                    deleteAlarmView->resetUpdateFlag();
                }

                if (deleteAlarmView->isBackRequested()) {
                    deleteAlarmView->resetBackRequest();
                    setView(View::Menu);
                }
                break;
            case View::AlarmTriggered:

                alarmTriggeredView->render(lcd);

                // Stop alarm if any button pressed
                if (buttons.anyPressed()) {
                    gpio_put(25, 0);                // Light off
                    clock.pauseTriggeredAlarms();
                    alarmTriggeredView->reset();
                    setView(View::Home);
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
            homeView->render(lcd);
            break;
        case View::Menu:
            menuView->resetUpdateFlag();
            menuView->render(lcd);
            break;
        case View::CreateAlarm:
            createAlarmView->resetUpdateFlag();
            createAlarmView->render(lcd);
            break;
        case View::ListAlarms:
            listAlarmsView->resetUpdateFlag();
            listAlarmsView->render(lcd);
            break;
        case View::DeleteAlarms:
            deleteAlarmView->resetUpdateFlag();
            deleteAlarmView->render(lcd);
            break;
        case View::AlarmTriggered:
            alarmTriggeredView->render(lcd);
            break;
        default:
            break;
    }
}

void AlarmApp::render() {
    views[currentView]->render(lcd);
}
