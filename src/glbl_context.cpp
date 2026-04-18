/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#include "glbl_context.h"

#include "audio/drivers/AudioDriver_ESP32.h"
#include "audio/drivers/AudioDriver_Null.h"
#include "audio/drivers/AudioDriver_RP2350.h"

namespace {

// Select the concrete driver once per target so the rest of the code can use
// the common IAudioDriver interface without preprocessor checks everywhere.
audio::IAudioDriver& selectAudioDriver()
{
#if defined(ARDUINO_ARCH_ESP32)
    static audio::AudioDriverESP32 driver;
    return driver;
#elif defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_ARCH_RP2350)
    static audio::AudioDriverRP2350 driver;
    return driver;
#else
    static audio::AudioDriverNull driver;
    return driver;
#endif
}

}  // namespace

SystemContext& systemContext()
{
    // Function-local static storage gives the firmware one shared context
    // without exposing mutable globals in every translation unit.
    static SystemContext context = {
        LedDriver595(),
        Led(),
        DebouncedButton(),
        ButtonEventGenerator(),
        EventQueue(),
        nullptr,
        audio::AudioOutput(),
        0,
        false,
    };

    if (context.audioDriver == nullptr) {
        context.audioDriver = &selectAudioDriver();
    }

    return context;
}
