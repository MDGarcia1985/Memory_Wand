/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#include "drivers/LedDriver595.h"
#include <Arduino.h>

LedDriver595::LedDriver595()
    : dataPin_(255),
      clockPin_(255),
      latchPin_(255),
      state_(0),
      initialized_(false) {}

void LedDriver595::begin(uint8_t dataPin, uint8_t clockPin, uint8_t latchPin) {
    dataPin_ = dataPin;
    clockPin_ = clockPin;
    latchPin_ = latchPin;

    // A 74HC595 uses three MCU signals:
    // data shifts one bit in, clock advances the register, and latch makes the
    // completed byte appear on the output pins all at once.
    pinMode(dataPin_, OUTPUT);
    pinMode(clockPin_, OUTPUT);
    pinMode(latchPin_, OUTPUT);

    initialized_ = true;
    clear();
}

void LedDriver595::setLed(uint8_t index, bool on) {
    if (index > 7) {
        return;
    }

    if (on) {
        // Each bit in `state_` represents one 74HC595 output pin.
        state_ |= (1 << index);
    } else {
        state_ &= ~(1 << index);
    }

    write();
}

void LedDriver595::toggleLed(uint8_t index) {
    if (index > 7) {
        return;
    }

    state_ ^= (1 << index);
    write();
}

void LedDriver595::clear() {
    state_ = 0;
    write();
}

void LedDriver595::write() const {
    if (!initialized_) {
        return;
    }

    // The cached byte is pushed MSB-first into the shift register. Toggling the
    // latch after shifting makes the full update visible in one step.
    digitalWrite(latchPin_, LOW);
    shiftOut(dataPin_, clockPin_, MSBFIRST, state_);
    digitalWrite(latchPin_, HIGH);
}

bool LedDriver595::getLed(uint8_t index) const {
    if (index > 7) {
        return false;
    }

    return (state_ & (1 << index)) != 0;
}
