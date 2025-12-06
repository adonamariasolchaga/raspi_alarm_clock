#include "button_manager.hpp"

ButtonManager::ButtonManager(uint button_up_pin, uint button_left_pin, uint button_right_pin,
                             uint button_bottom_pin, uint button_center_pin)
    : up_pin(button_up_pin),
      left_pin(button_left_pin),
      right_pin(button_right_pin),
      bottom_pin(button_bottom_pin),
      pin_ok(button_center_pin) {}

void ButtonManager::init() {
    gpio_init(up_pin);
    gpio_set_dir(up_pin, GPIO_IN);
    gpio_pull_up(up_pin);

    gpio_init(bottom_pin);
    gpio_set_dir(bottom_pin, GPIO_IN);
    gpio_pull_up(bottom_pin);

    gpio_init(left_pin);
    gpio_set_dir(left_pin, GPIO_IN);
    gpio_pull_up(left_pin);

    gpio_init(right_pin);
    gpio_set_dir(right_pin, GPIO_IN);
    gpio_pull_up(right_pin);

    gpio_init(pin_ok);
    gpio_set_dir(pin_ok, GPIO_IN);
    gpio_pull_up(pin_ok);
}

bool ButtonManager::is_pressedU() {
    return gpio_get(up_pin) == 0;
}

bool ButtonManager::is_pressedD() {
    return gpio_get(bottom_pin) == 0;
}

bool ButtonManager::is_pressedL() {
    return gpio_get(left_pin) == 0;
}

bool ButtonManager::is_pressedR() {
    return gpio_get(right_pin) == 0;
}

bool ButtonManager::is_pressedC() {
    return gpio_get(pin_ok) == 0;
}

bool ButtonManager::anyPressed() {
    return is_pressedU() || is_pressedD() || is_pressedL() || is_pressedR() || is_pressedC();
}
