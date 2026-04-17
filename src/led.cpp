/*
 * This is for LEDs implimentation and logic that is then passed to main.cpp through headers.h
 * This project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY;
 * 
 * Author:  Michael Garcia
 * Email:   michael@mandedesign.studio
 * website: https://mandedesign.studio
 * Date:    2026-04-11
 * Version: 1.0
*/
#include "led.h"
#include <Arduino.h>

Led::Led()
    : pin_(255),
      activeHigh_(true),
      state_(false),
      blinking_(false),
      // Initialize to known safe state (0) to prevent undefined timing behavior.
      onMs_(0),
      offMs_(0),
      lastChangeMs_(0) {}

void Led::begin(uint8_t pin, bool activeHigh) {
    pin_ = pin;
    activeHigh_ = activeHigh;
    pinMode(pin_, OUTPUT);
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
    if (pin_ == 255) {
        return;
    }

    bool level = activeHigh_ ? on : !on;
    digitalWrite(pin_, level ? HIGH : LOW);
}