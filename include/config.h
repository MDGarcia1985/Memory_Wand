/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

namespace config {

/**
 * Fixed capacities used by the firmware's static storage.
 * These are compile-time sizes because the project avoids heap allocation in
 * core runtime paths.
 */
constexpr size_t kButtonCount = 1;
constexpr size_t kLedCount = 1;
constexpr size_t kEventQueueCapacity = 16;
constexpr size_t kAudioChannelCount = 2;

/**
 * Timing and debounce policy for button sampling and event generation.
 * All time values are in milliseconds unless the field name states otherwise.
 */
struct ButtonConfig {
    uint32_t pollIntervalMs;
    uint32_t holdThresholdMs;
    bool activeLow;
    bool useInternalPullup;
    uint8_t integratorMax;
    uint8_t pressThreshold;
    uint8_t releaseThreshold;
};

/**
 * LED behavior policy that tasks can apply without hard-coded durations.
 */
struct LedConfig {
    uint32_t holdBlinkOnMs;
    uint32_t holdBlinkOffMs;
};

/**
 * System-wide runtime configuration shared across modules.
 * This keeps behavioral tuning in one place instead of scattering literals
 * through the task and driver code.
 */
struct SystemConfig {
    ButtonConfig button;
    LedConfig led;
};

/**
 * Returns the central runtime configuration for the current firmware target.
 */
const SystemConfig& getSystemConfig();

}  // namespace config
