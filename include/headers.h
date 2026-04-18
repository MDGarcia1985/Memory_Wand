/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#pragma once

/**
 * Aggregated include for the firmware application entry point.
 * Modules should usually include only the headers they directly need.
 */

#include "audio/AudioConfig.h"
#include "audio/AudioOutput.h"
#include "audio/AudioTypes.h"
#include "audio/GeneratedSoundData.h"
#include "audio/IAudioDriver.h"
#include "btn_debounce.h"
#include "btn_event.h"
#include "config.h"
#include "drivers/LedDriver595.h"
#include "event_queue.h"
#include "glbl_context.h"
#include "led.h"
#include "pins.h"
#include "tasks.h"
