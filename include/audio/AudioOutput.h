/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#pragma once

#include <stdint.h>

#include "audio/AudioTypes.h"
#include "config.h"

namespace audio {

class IAudioDriver;

/**
 * Non-blocking audio sequencer that advances generated note data over time.
 *
 * `AudioOutput` sits between high-level requests like "play ButtonPress" and
 * low-level driver calls like "output 659 Hz on the speaker pin".
 */
class AudioOutput {
public:
    AudioOutput();

    /**
     * Injects the hardware driver chosen for the current platform.
     */
    void begin(IAudioDriver& driver);

    /**
     * Starts one logical sound definition on its assigned channel.
     * Returns false if the sound is missing or loses a priority arbitration.
     */
    bool playSound(SoundId id, uint32_t nowMs);
    void stopAll();

    /**
     * Advances active channels according to `millis()` time and transitions to
     * the next note whenever the current note duration expires.
     */
    void update(uint32_t nowMs);

private:
    void startChannelNote(AudioChannel& channel, uint32_t nowMs);
    void stopChannel(AudioChannel& channel);

    IAudioDriver* driver_;
    AudioChannel channels_[config::kAudioChannelCount];
    bool initialized_;
};

}  // namespace audio
