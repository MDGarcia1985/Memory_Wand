/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#include "audio/GeneratedSoundData.h"

namespace audio {
namespace {

// In the long-term architecture this file is generated from JSON. For now it
// serves as the same read-only interface the generated output would provide.
constexpr NoteEvent kStartupNotes[] = {
    {523, 90},
    {659, 90},
    {784, 140},
};

constexpr NoteEvent kButtonPressNotes[] = {
    {659, 70},
};

constexpr NoteEvent kButtonReleaseNotes[] = {
    {440, 60},
};

constexpr NoteEvent kButtonHoldNotes[] = {
    {523, 90},
    {0, 30},
    {659, 120},
};

constexpr SoundDef kSoundTable[] = {
    {SoundId::Startup, kStartupNotes, sizeof(kStartupNotes) / sizeof(kStartupNotes[0]), 0, 10, false},
    {SoundId::ButtonPress, kButtonPressNotes, sizeof(kButtonPressNotes) / sizeof(kButtonPressNotes[0]), 0, 20, false},
    {SoundId::ButtonRelease, kButtonReleaseNotes, sizeof(kButtonReleaseNotes) / sizeof(kButtonReleaseNotes[0]), 0, 15, false},
    {SoundId::ButtonHold, kButtonHoldNotes, sizeof(kButtonHoldNotes) / sizeof(kButtonHoldNotes[0]), 0, 30, false},
};

}  // namespace

const SoundDef* findSoundDef(SoundId id)
{
    // A simple linear scan is acceptable here because the sound table is small
    // and fixed at compile time.
    for (const SoundDef& sound : kSoundTable) {
        if (sound.id == id) {
            return &sound;
        }
    }

    return nullptr;
}

}  // namespace audio
