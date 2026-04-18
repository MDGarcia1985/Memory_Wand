/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#pragma once

#include <stdint.h>

class LedDriver595 {
public:
    LedDriver595();

    /**
     * Configures the three MCU pins used to talk to one 74HC595 shift register.
     * `dataPin` carries the next bit, `clockPin` shifts it in, and `latchPin`
     * publishes the completed byte to the physical output pins.
     */
    void begin(uint8_t dataPin, uint8_t clockPin, uint8_t latchPin);

    /**
     * Updates one output bit in the driver's cached state byte and immediately
     * writes the new byte to the shift register.
     */
    void setLed(uint8_t index, bool on);
    void toggleLed(uint8_t index);
    void clear();

    /**
     * Pushes the cached output byte into the 74HC595 hardware.
     */
    void write() const;

    bool getLed(uint8_t index) const;

private:
    uint8_t dataPin_;
    uint8_t clockPin_;
    uint8_t latchPin_;
    uint8_t state_;
    bool initialized_;
};
