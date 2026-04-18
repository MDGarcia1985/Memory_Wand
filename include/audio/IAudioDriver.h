/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#pragma once

#include <stdint.h>

#include "audio/AudioConfig.h"

namespace audio {

/**
 * Platform abstraction for sending audio output to the current hardware target.
 *
 * Everything above this interface speaks in logical audio terms.
 * Everything below this interface is free to use board-specific PWM, timers,
 * or even a no-op implementation for tests.
 */
class IAudioDriver {
public:
    virtual ~IAudioDriver() = default;

    /**
     * Performs one-time hardware setup for the selected audio output path.
     */
    virtual void begin(const AudioHardwareConfig& config) = 0;

    /**
     * Requests one output frequency in Hertz from the hardware layer.
     */
    virtual void writeFrequency(uint16_t frequencyHz) = 0;

    /**
     * Silences the output path.
     */
    virtual void stop() = 0;
    virtual void setVolume(uint8_t volume) = 0;
};

}  // namespace audio
