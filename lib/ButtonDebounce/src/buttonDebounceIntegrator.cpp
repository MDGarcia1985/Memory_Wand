/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#include "ButtonDebounce.h"

ButtonDebounce::ButtonDebounce(const Config& cfg) : cfg_(cfg)
{
    reset(false);
}

void ButtonDebounce::reset(bool start_down)
{
    state_ = start_down;
    pressed_ = false;
    released_ = false;
    eng_.integrator.acc = state_ ? cfg_.integ_max : 0u;
}

void ButtonDebounce::update(bool raw_down)
{
    pressed_ = false;
    released_ = false;

    // The integrator engine does not trust a single sample. It accumulates
    // evidence toward "pressed" or "released" over several scheduler ticks.
    if (raw_down) {
        if (eng_.integrator.acc < cfg_.integ_max) {
            eng_.integrator.acc++;
        }
    } else if (eng_.integrator.acc > 0u) {
        eng_.integrator.acc--;
    }

    // Separate on/off thresholds create hysteresis, which prevents the state
    // from chattering if the accumulator hovers near one boundary.
    if (!state_ && eng_.integrator.acc >= cfg_.integ_on) {
        state_ = true;
        pressed_ = true;
    } else if (state_ && eng_.integrator.acc <= cfg_.integ_off) {
        state_ = false;
        released_ = true;
    }
}

uint8_t ButtonDebounce::history() const
{
    return 0u;
}
