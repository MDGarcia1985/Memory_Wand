/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#include "audio/drivers/AudioDriver_ESP32.h"

#include <Arduino.h>

namespace audio {

AudioDriverESP32::AudioDriverESP32() : volume_(255), initialized_(false)
{
}

void AudioDriverESP32::begin(const AudioHardwareConfig& config)
{
    config_ = config;
    initialized_ = config_.enabled;

    if (!initialized_) {
        return;
    }

    // Even though this driver is still a stub for actual tone generation, it
    // already owns the electrical setup of the output path for ESP32 targets.
    pinMode(config_.speakerPin, OUTPUT);
    if (config_.amplifierEnablePin >= 0) {
        pinMode(config_.amplifierEnablePin, OUTPUT);
        digitalWrite(config_.amplifierEnablePin, config_.amplifierEnableActiveHigh ? HIGH : LOW);
    }
}

void AudioDriverESP32::writeFrequency(uint16_t frequencyHz)
{
    (void)frequencyHz;
}

void AudioDriverESP32::stop()
{
}

void AudioDriverESP32::setVolume(uint8_t volume)
{
    volume_ = volume;
    (void)volume_;
}

}  // namespace audio
