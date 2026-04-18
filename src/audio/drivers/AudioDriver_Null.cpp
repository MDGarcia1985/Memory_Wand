/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#include "audio/drivers/AudioDriver_Null.h"

namespace audio {

void AudioDriverNull::begin(const AudioHardwareConfig& config)
{
    // The null driver keeps the same interface contract as real drivers while
    // intentionally producing no hardware output.
    config_ = config;
}

void AudioDriverNull::writeFrequency(uint16_t frequencyHz)
{
    (void)frequencyHz;
}

void AudioDriverNull::stop()
{
}

void AudioDriverNull::setVolume(uint8_t volume)
{
    volume_ = volume;
}

}  // namespace audio
