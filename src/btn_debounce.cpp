/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#include "btn_debounce.h"

#include <Arduino.h>

namespace {

// Translate project-level tuning values into the external library's config type.
ButtonDebounce::Config makeDebounceConfig(const config::ButtonConfig& buttonConfig)
{
    ButtonDebounce::Config config{};
    config.integ_max = buttonConfig.integratorMax;
    config.integ_on = buttonConfig.pressThreshold;
    config.integ_off = buttonConfig.releaseThreshold;
    return config;
}

}  // namespace

DebouncedButton::DebouncedButton()
    : pin_(0),
      activeLow_(true),
      configured_(false),
      debouncer_(ButtonDebounce::Config{})
{
}

void DebouncedButton::begin(uint8_t pin, const config::ButtonConfig& buttonConfig)
{
    pin_ = pin;
    activeLow_ = buttonConfig.activeLow;
    configured_ = true;
    debouncer_ = ButtonDebounce(makeDebounceConfig(buttonConfig));

    // The pin mode is part of the hardware contract. Once this is configured,
    // the rest of the system can treat the button as a logical input source.
    pinMode(pin_, buttonConfig.useInternalPullup ? INPUT_PULLUP : INPUT);
    debouncer_.reset(false);
}

void DebouncedButton::update()
{
    if (!configured_) {
        return;
    }

    const bool pinHigh = (digitalRead(pin_) != LOW);
    if (activeLow_) {
        // Active-low wiring is common in embedded boards because pull-ups make
        // the idle electrical state predictable. The wrapper hides that detail.
        debouncer_.updateActiveLow(pinHigh);
        return;
    }

    debouncer_.updateActiveHigh(pinHigh);
}

bool DebouncedButton::pressed() const
{
    return debouncer_.pressed();
}

bool DebouncedButton::released() const
{
    return debouncer_.released();
}

bool DebouncedButton::down() const
{
    return debouncer_.down();
}

bool DebouncedButton::up() const
{
    return debouncer_.up();
}
