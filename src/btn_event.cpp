/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#include "btn_event.h"

#include "btn_debounce.h"

ButtonEventGenerator::ButtonEventGenerator()
    : buttonId_(ButtonId::Action),
      holdThresholdMs_(0),
      pressedAtMs_(0),
      configured_(false),
      holdReported_(false)
{
}

void ButtonEventGenerator::begin(ButtonId buttonId, uint32_t holdThresholdMs)
{
    buttonId_ = buttonId;
    holdThresholdMs_ = holdThresholdMs;
    pressedAtMs_ = 0;
    configured_ = true;
    holdReported_ = false;
}

bool ButtonEventGenerator::update(const DebouncedButton& button, uint32_t nowMs, ButtonEvent& eventOut)
{
    if (!configured_) {
        return false;
    }

    // Edge events are emitted first so a press or release is never hidden by
    // hold detection during the same scheduler tick.
    if (button.pressed()) {
        pressedAtMs_ = nowMs;
        holdReported_ = false;
        eventOut = {buttonId_, ButtonEventType::Pressed, nowMs};
        return true;
    }

    if (button.released()) {
        holdReported_ = false;
        eventOut = {buttonId_, ButtonEventType::Released, nowMs};
        return true;
    }

    // A hold is a derived event: the button must still be down and must have
    // remained down long enough since the most recent press.
    if (button.down() && !holdReported_ && holdThresholdMs_ > 0 &&
        (nowMs - pressedAtMs_) >= holdThresholdMs_) {
        holdReported_ = true;
        eventOut = {buttonId_, ButtonEventType::Held, nowMs};
        return true;
    }

    return false;
}
