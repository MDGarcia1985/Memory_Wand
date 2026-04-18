/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#include "audio/drivers/AudioDriver_RP2350.h"

#include <Arduino.h>

namespace audio {

AudioDriverRP2350::AudioDriverRP2350() : volume_(255), initialized_(false)
{
}

void AudioDriverRP2350::begin(const AudioHardwareConfig& config)
{
    config_ = config;
    initialized_ = config_.enabled;

    if (!initialized_) {
        return;
    }

    // Like the ESP32 driver, this currently establishes pin ownership and the
    // abstraction boundary even before a final PWM implementation is added.
    pinMode(config_.speakerPin, OUTPUT);
    if (config_.amplifierEnablePin >= 0) {
        pinMode(config_.amplifierEnablePin, OUTPUT);
        digitalWrite(config_.amplifierEnablePin, config_.amplifierEnableActiveHigh ? HIGH : LOW);
    }
}

void AudioDriverRP2350::writeFrequency(uint16_t frequencyHz)
{
    (void)frequencyHz;
}

void AudioDriverRP2350::stop()
{
}

void AudioDriverRP2350::setVolume(uint8_t volume)
{
    volume_ = volume;
    (void)volume_;
}

}  // namespace audio
