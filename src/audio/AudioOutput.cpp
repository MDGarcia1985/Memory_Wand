/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#include "audio/AudioOutput.h"

#include "audio/GeneratedSoundData.h"
#include "audio/IAudioDriver.h"

namespace audio {

AudioOutput::AudioOutput() : driver_(nullptr), initialized_(false)
{
    for (AudioChannel& channel : channels_) {
        channel = {false, nullptr, 0, 0, 0};
    }
}

void AudioOutput::begin(IAudioDriver& driver)
{
    driver_ = &driver;
    initialized_ = true;

    for (AudioChannel& channel : channels_) {
        channel = {false, nullptr, 0, 0, 0};
    }
}

bool AudioOutput::playSound(SoundId id, uint32_t nowMs)
{
    if (!initialized_) {
        return false;
    }

    const SoundDef* sound = findSoundDef(id);
    if (sound == nullptr || sound->channel >= config::kAudioChannelCount) {
        return false;
    }

    AudioChannel& channel = channels_[sound->channel];
    // Higher-priority sounds are allowed to interrupt lower-priority sounds on
    // the same logical channel. Lower-priority requests are ignored.
    if (channel.active && channel.priority > sound->priority) {
        return false;
    }

    channel.active = true;
    channel.sound = sound;
    channel.noteIndex = 0;
    channel.noteStartedAtMs = nowMs;
    channel.priority = sound->priority;
    startChannelNote(channel, nowMs);
    return true;
}

void AudioOutput::stopAll()
{
    if (driver_ != nullptr) {
        driver_->stop();
    }

    for (AudioChannel& channel : channels_) {
        stopChannel(channel);
    }
}

void AudioOutput::update(uint32_t nowMs)
{
    if (!initialized_ || driver_ == nullptr) {
        return;
    }

    for (AudioChannel& channel : channels_) {
        if (!channel.active || channel.sound == nullptr) {
            continue;
        }

        const NoteEvent& note = channel.sound->notes[channel.noteIndex];
        if ((nowMs - channel.noteStartedAtMs) < note.durationMs) {
            continue;
        }

        // Once a note duration expires, advance to the next note definition in
        // the generated pattern. Looping sounds wrap back to the first note.
        channel.noteIndex++;
        if (channel.noteIndex >= channel.sound->noteCount) {
            if (channel.sound->loop) {
                channel.noteIndex = 0;
            } else {
                stopChannel(channel);
                driver_->stop();
                continue;
            }
        }

        startChannelNote(channel, nowMs);
    }
}

void AudioOutput::startChannelNote(AudioChannel& channel, uint32_t nowMs)
{
    if (driver_ == nullptr || channel.sound == nullptr || channel.noteIndex >= channel.sound->noteCount) {
        return;
    }

    channel.noteStartedAtMs = nowMs;
    const NoteEvent& note = channel.sound->notes[channel.noteIndex];
    if (note.frequencyHz == 0) {
        // A zero-frequency note is treated as a rest, which lets the generated
        // sound data describe audible gaps without special control messages.
        driver_->stop();
        return;
    }

    driver_->writeFrequency(note.frequencyHz);
}

void AudioOutput::stopChannel(AudioChannel& channel)
{
    channel.active = false;
    channel.sound = nullptr;
    channel.noteIndex = 0;
    channel.noteStartedAtMs = 0;
    channel.priority = 0;
}

}  // namespace audio
