/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#include "led.h"
#include "drivers/LedDriver595.h"
#include <Arduino.h>

Led::Led()
    : ledIndex_(255),
      driver_(nullptr),
      activeHigh_(true),
      state_(false),
      blinking_(false),
      // Known start state to avoid undefined behavior before configuration.
      onMs_(0),
      offMs_(0),
      lastChangeMs_(0) {}

void Led::begin(uint8_t ledIndex, LedDriver595* driver, bool activeHigh) {
    ledIndex_ = ledIndex;
    driver_ = driver;
    activeHigh_ = activeHigh;
    off();
}

void Led::on() {
    blinking_ = false;
    state_ = true;
    writePin(true);
}

void Led::off() {
    blinking_ = false;
    state_ = false;
    writePin(false);
}

void Led::toggle() {
    state_ = !state_;
    writePin(state_);
}

void Led::blink(uint32_t onMs, uint32_t offMs) {
    onMs_ = onMs;
    offMs_ = offMs;
    blinking_ = true;
    state_ = true;
    lastChangeMs_ = millis();
    writePin(true);
}

void Led::stopBlink() {
    blinking_ = false;
}

void Led::update() {
    if (!blinking_) {
        return;
    }

    uint32_t now = millis();
    uint32_t interval = state_ ? onMs_ : offMs_;

    if (interval == 0) {
        return;
    }

    if (now - lastChangeMs_ >= interval) {
        state_ = !state_;
        lastChangeMs_ = now;
        writePin(state_);
    }
}

bool Led::isOn() const {
    return state_;
}

bool Led::isBlinking() const {
    return blinking_;
}

void Led::writePin(bool on) {
    if (driver_ == nullptr || ledIndex_ == 255) {
        return;
    }

    bool level = activeHigh_ ? on : !on;
    driver_->setLed(ledIndex_, level);
}