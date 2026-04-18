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
 * ESP32-family audio driver boundary.
 * The implementation currently preserves the hardware abstraction seam and
 * board pin setup without committing to a specific PWM strategy yet.
 */
class AudioDriverESP32 : public IAudioDriver {
public:
    AudioDriverESP32();

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
