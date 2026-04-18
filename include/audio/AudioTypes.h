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

namespace audio {

/**
 * Symbolic sound IDs used by tasks and gameplay code.
 * These names are what the rest of the firmware should reference instead of
 * hard-coding note tables directly.
 */
enum class SoundId : uint8_t {
    Startup = 0,
    ButtonPress,
    ButtonRelease,
    ButtonHold,
};

/**
 * One timed note or silence step in a sound pattern.
 * `frequencyHz == 0` means "rest" rather than "play an inaudible tone".
 */
struct NoteEvent {
    uint16_t frequencyHz;
    uint16_t durationMs;
};

/**
 * Read-only sound definition generated from data files.
 * `channel` and `priority` are consumed by AudioOutput when multiple sounds
 * compete for the same physical output path.
 */
struct SoundDef {
    SoundId id;
    const NoteEvent* notes;
    size_t noteCount;
    uint8_t channel;
    uint8_t priority;
    bool loop;
};

/**
 * Runtime channel state managed by AudioOutput.
 * This is mutable playback state, not design-time sound data.
 */
struct AudioChannel {
    bool active;
    const SoundDef* sound;
    size_t noteIndex;
    uint32_t noteStartedAtMs;
    uint8_t priority;
};

}  // namespace audio
