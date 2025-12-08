
# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [unreleased]

### Added

### Changed

### Removed

## [3.0.0] - 2025-12-08

### Added

- Add `initial_setup_view` module in order to set the initial time for the alarm.
- Add `alarm_clock_v3_0_0.uf2` binary file with the new released version.

### Changed

- Change `src/main.cpp` entry point to launch the initial date/time setting view before launching the `run()` method.

## [2.0.0] - 2025-12-06

### Added

- Add `buzzer` module managing active buzzer bahaviour.

### Changed

- Change `AlarmApp::run` method in order to activate / deactivate sound when alarm is triggered.


## [1.0.0] - 2025-12-06

### Added

- Add compiled application in `compiled/alarm_clock_v1_0_0.uf2`.
- Add `HomeView` rendering the actual time from `12:00:00` forward and giving access to `ScrollableMenuView` via `>Menu` button.
- Add `ScrollableMenuView` giving access to the following features:
  - Create alarms
  - List existing alarms
  - Remove alarms
  - Return to `HomeView`
- Add `CreateAlarmView` enabling the creation of alarms by setting their hour and minute using the left and right buttons. Save/cancel options are available.
- Add `ListAlarmsView` to browse between the existing alarms in the system.
- Add `DeleteAlarmView` to select any existing alarm and erase it.
- Add `AlarmTriggeredView` when an alarm is triggered. It blinks indicating the alarm triggered and return to `HomeView` if anny button is pressed.
