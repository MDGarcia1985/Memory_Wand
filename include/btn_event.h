/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#pragma once

#include <stdint.h>

class DebouncedButton;

/**
 * Semantic button identity used after the hardware pin has already been mapped.
 * This lets the rest of the system talk about "which logical button" instead
 * of carrying raw GPIO numbers through the event pipeline.
 */
enum class ButtonId : uint8_t {
    Action = 0,
};

/**
 * High-level button transitions produced by the button event layer.
 * These are intentionally richer than raw electrical state changes.
 */
enum class ButtonEventType : uint8_t {
    Pressed = 0,
    Released,
    Held,
};

/**
 * Debounced button event with a timestamp captured at the point of generation.
 * The timestamp is in `millis()` units so tasks can make timing decisions
 * without needing to re-read the hardware clock.
 */
struct ButtonEvent {
    ButtonId button;
    ButtonEventType type;
    uint32_t timestampMs;
};

/**
 * Converts debounced button state into explicit press, release, and hold events.
 *
 * The debounce layer answers "is the button stable?".
 * This layer answers "what meaningful interaction just happened?" and packages
 * that result for the event queue.
 */
class ButtonEventGenerator {
public:
    ButtonEventGenerator();

    /**
     * Configures which logical button this generator represents and how long
     * the button must remain held before a HOLD event is emitted.
     */
    void begin(ButtonId buttonId, uint32_t holdThresholdMs);

    /**
     * Examines the current debounced button state and emits at most one event.
     * Limiting this to one event per call keeps queue ordering deterministic.
     */
    bool update(const DebouncedButton& button, uint32_t nowMs, ButtonEvent& eventOut);

private:
    ButtonId buttonId_;
    uint32_t holdThresholdMs_;
    uint32_t pressedAtMs_;
    bool configured_;
    bool holdReported_;
};
