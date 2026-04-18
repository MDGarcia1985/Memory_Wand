/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#pragma once

#include "audio/AudioOutput.h"
#include "audio/IAudioDriver.h"
#include "btn_debounce.h"
#include "btn_event.h"
#include "drivers/LedDriver595.h"
#include "event_queue.h"
#include "led.h"

/**
 * Shared runtime state for the firmware.
 *
 * Embedded projects often end up with many file-scope globals. This struct
 * keeps subsystem ownership explicit by grouping the long-lived objects that
 * `setup()`, `loop()`, and the task layer all need to coordinate.
 */
struct SystemContext {
    // Shift-register backend that owns the actual 74HC595 output byte.
    LedDriver595 ledDriver;

    // Semantic LED object layered on top of the backend driver.
    Led statusLed;

    // Input stack: raw GPIO -> debouncer -> event generator.
    DebouncedButton actionButton;
    ButtonEventGenerator actionButtonEvents;

    // Cross-module handoff point between inputs and behaviors.
    EventQueue eventQueue;

    // Audio driver is selected per-target at runtime and then owned here.
    audio::IAudioDriver* audioDriver;
    audio::AudioOutput audioOutput;

    // Millisecond timestamp of the last scheduled button sample.
    uint32_t lastButtonPollMs;
    bool initialized;
};

/**
 * Returns the single runtime context used by the firmware.
 */
SystemContext& systemContext();
