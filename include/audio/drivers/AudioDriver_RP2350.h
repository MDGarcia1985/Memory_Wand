/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#pragma once

#include "audio/IAudioDriver.h"

namespace audio {

/**
 * RP2350-family audio driver boundary.
 */
class AudioDriverRP2350 : public IAudioDriver {
public:
    AudioDriverRP2350();

    void begin(const AudioHardwareConfig& config) override;
    void writeFrequency(uint16_t frequencyHz) override;
    void stop() override;
    void setVolume(uint8_t volume) override;

private:
    AudioHardwareConfig config_;
    uint8_t volume_;
    bool initialized_;
};

}  // namespace audio
