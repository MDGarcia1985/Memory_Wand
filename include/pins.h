/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#pragma once

#include <stdint.h>

namespace pins {

/**
 * Logical GPIO assignments for the current hardware target.
 * These values are the only place that should know the actual pin numbers.
 */
struct PinAssignments {
    uint8_t buttonInput;
    uint8_t shiftRegisterData;
    uint8_t shiftRegisterClock;
    uint8_t shiftRegisterLatch;
    uint8_t speakerOutput;
    int8_t amplifierEnable;
};

/**
 * Shift-register LED indices. These are semantic positions within the 74HC595.
 */
enum class LedIndex : uint8_t {
    Status = 0,
};

/**
 * Returns the active pin mapping for this build.
 * Keeping this behind a function makes it easy to swap mappings per board.
 */
const PinAssignments& getPinAssignments();

}  // namespace pins
