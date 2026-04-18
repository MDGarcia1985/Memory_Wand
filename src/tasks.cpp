/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#include "tasks.h"

#include "audio/AudioConfig.h"
#include "config.h"
#include "pins.h"

namespace {

// This helper translates queued button interactions into visible or audible
// system behavior. It is where "an event happened" becomes "the toy reacts".
void handleButtonEvent(SystemContext& context, const ButtonEvent& buttonEvent, uint32_t nowMs)
{
    const auto& systemConfig = config::getSystemConfig();

    switch (buttonEvent.type) {
        case ButtonEventType::Pressed:
            context.statusLed.on();
            context.audioOutput.playSound(audio::SoundId::ButtonPress, nowMs);
            break;

        case ButtonEventType::Released:
            context.statusLed.off();
            context.audioOutput.playSound(audio::SoundId::ButtonRelease, nowMs);
            break;

        case ButtonEventType::Held:
            context.statusLed.blink(systemConfig.led.holdBlinkOnMs, systemConfig.led.holdBlinkOffMs);
            context.audioOutput.playSound(audio::SoundId::ButtonHold, nowMs);
            break;
    }
}

}  // namespace

void initializeSystem(SystemContext& context, uint32_t nowMs)
{
    if (context.initialized) {
        return;
    }

    // Hardware and shared subsystems are initialized exactly once, even though
    // the task pipeline calls this function every loop iteration.
    const auto& boardPins = pins::getPinAssignments();
    const auto& systemConfig = config::getSystemConfig();

    context.ledDriver.begin(
        boardPins.shiftRegisterData,
        boardPins.shiftRegisterClock,
        boardPins.shiftRegisterLatch);

    context.statusLed.begin(
        static_cast<uint8_t>(pins::LedIndex::Status),
        &context.ledDriver,
        true);

    context.actionButton.begin(boardPins.buttonInput, systemConfig.button);
    context.actionButtonEvents.begin(ButtonId::Action, systemConfig.button.holdThresholdMs);

    if (context.audioDriver != nullptr) {
        context.audioDriver->begin(audio::getAudioHardwareConfig());
        context.audioOutput.begin(*context.audioDriver);
        context.audioOutput.playSound(audio::SoundId::Startup, nowMs);
    }

    context.lastButtonPollMs = nowMs;
    context.initialized = true;
}

void pollInputs(SystemContext& context, uint32_t nowMs)
{
    const auto& systemConfig = config::getSystemConfig();
    if ((nowMs - context.lastButtonPollMs) < systemConfig.button.pollIntervalMs) {
        return;
    }

    // Input polling is rate-limited so the debouncer sees a predictable sample
    // interval instead of a loop frequency that changes with other workload.
    context.lastButtonPollMs = nowMs;
    context.actionButton.update();

    ButtonEvent buttonEvent{};
    if (context.actionButtonEvents.update(context.actionButton, nowMs, buttonEvent)) {
        context.eventQueue.push(SystemEvent::fromButtonEvent(buttonEvent));
    }
}

void processEvents(SystemContext& context, uint32_t nowMs)
{
    SystemEvent event{};

    // Drain the queue completely each pass so input handling stays decoupled
    // from behavior execution without leaving stale events behind.
    while (context.eventQueue.pop(event)) {
        switch (event.type) {
            case SystemEventType::Button:
                handleButtonEvent(context, event.button, nowMs);
                break;
        }
    }
}

void updateOutputs(SystemContext& context, uint32_t nowMs)
{
    // These updates are intentionally non-blocking. Each subsystem advances its
    // own internal timing and returns immediately.
    context.statusLed.update();
    context.audioOutput.update(nowMs);
}

void runTasks(SystemContext& context, uint32_t nowMs)
{
    initializeSystem(context, nowMs);
    pollInputs(context, nowMs);
    processEvents(context, nowMs);
    updateOutputs(context, nowMs);
}
