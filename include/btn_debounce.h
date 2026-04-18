/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#pragma once

#include <stdint.h>

#include "ButtonDebounce.h"

#include "config.h"

/**
 * Reads one physical button and exposes debounced edge and level state.
 *
 * This wrapper is the boundary between raw GPIO reads and the rest of the
 * firmware. Code outside this file should not need to know whether the button
 * is wired active-low, whether a pull-up resistor is enabled, or which
 * debouncing engine is being used.
 */
class DebouncedButton {
public:
    DebouncedButton();

    /**
     * Binds this logical button to one MCU input pin and one debounce policy.
     * `pin` is a physical GPIO number from pins.cpp, not a semantic button ID.
     */
    void begin(uint8_t pin, const config::ButtonConfig& buttonConfig);

    /**
     * Samples the raw pin and advances the debounce state machine once.
     * This should be called on a fixed schedule from the task layer.
     */
    void update();

    // One-shot edge queries. These return true for a single poll interval.
    bool pressed() const;
    bool released() const;

    // Debounced level queries. These describe the current stable button state.
    bool down() const;
    bool up() const;

private:
    uint8_t pin_;
    bool activeLow_;
    bool configured_;
    ButtonDebounce debouncer_;
};
