#pragma once
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
#include <stdint.h>

class Led {
public:
    Led();
    // unit8_t is C++'s way of saying unsigned char. It allows the system to use a byte as a variable.
    void begin(uint8_t pin, bool activeHigh = true);

    void on();
    void off();
    void toggle();

    // uint32_t is C++'s way of saying unsigned int. It allows the system to use a 32 bit integer as a variable.
    void blink(uint32_t onMs, uint32_t offMs);
    void stopBlink();

    void update();

    bool isOn() const;
    bool isBlinking() const;

private:
    uint8_t pin_;
    bool activeHigh_;
    bool state_;
    bool blinking_;

    uint32_t onMs_;
    uint32_t offMs_;
    uint32_t lastChangeMs_;

    void writePin(bool on);
};