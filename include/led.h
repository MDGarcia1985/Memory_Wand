/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#pragma once

#include <stdint.h>

class LedDriver595;

/**
 * Semantic LED wrapper that owns non-blocking blink state while delegating
 * physical output to a backend driver.
 */
class Led {
public:
    Led();

    /**
     * For 74HC595-backed LEDs, ledIndex is the register bit index (0-7), not
     * a raw MCU GPIO number.
     */
    void begin(uint8_t ledIndex, LedDriver595* driver, bool activeHigh = true);

    void on();
    void off();
    void toggle();

    void blink(uint32_t onMs, uint32_t offMs);
    void stopBlink();

    void update();

    bool isOn() const;
    bool isBlinking() const;

private:
    uint8_t ledIndex_;
    LedDriver595* driver_;
    bool activeHigh_;
    bool state_;
    bool blinking_;
    uint32_t onMs_;
    uint32_t offMs_;
    uint32_t lastChangeMs_;

    void writePin(bool on);
};
