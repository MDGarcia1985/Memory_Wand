/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#pragma once

#include <stdint.h>

namespace audio {

/**
 * Board-specific electrical details required by the audio driver layer.
 * This keeps pin and PWM choices separate from the audio sequencing logic.
 */
struct AudioHardwareConfig {
    uint8_t speakerPin;
    int8_t amplifierEnablePin;
    bool amplifierEnableActiveHigh;
    uint8_t pwmChannel;
    uint8_t pwmResolutionBits;
    bool enabled;
};

/**
 * Returns the audio hardware configuration for the active target.
 * The audio engine uses this once during setup and then works in logical
 * units like sound IDs, channels, and note timings.
 */
const AudioHardwareConfig& getAudioHardwareConfig();

}  // namespace audio
