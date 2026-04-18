/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#pragma once

#include "audio/AudioTypes.h"

namespace audio {

/**
 * Returns the generated sound definition for a symbolic sound identifier.
 */
const SoundDef* findSoundDef(SoundId id);

}  // namespace audio
