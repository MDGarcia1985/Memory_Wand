/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#include "audio/AudioConfig.h"

#include "pins.h"

namespace audio {

const AudioHardwareConfig& getAudioHardwareConfig()
{
    const auto& boardPins = pins::getPinAssignments();

    // This is the audio-side view of the board wiring. AudioOutput does not
    // inspect pins directly; it receives this resolved hardware contract.
    static const AudioHardwareConfig kConfig = {
        boardPins.speakerOutput,
        boardPins.amplifierEnable,
        true,
        0,
        8,
        true,
    };

    return kConfig;
}

}  // namespace audio
