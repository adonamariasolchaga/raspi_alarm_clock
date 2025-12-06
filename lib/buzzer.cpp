#include "buzzer.hpp"

Buzzer::Buzzer(uint pin) : pin(pin)
{
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_OUT);
    gpio_put(pin, 0); // buzzer off by default
}

void Buzzer::on()
{
    gpio_put(pin, 1);
}

void Buzzer::off()
{
    gpio_put(pin, 0);
}
