/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#pragma once

#include <stdint.h>

#include "glbl_context.h"

/**
 * Performs one-time hardware and subsystem setup.
 * This is separated from `setup()` so the main loop can remain thin and the
 * initialization sequence stays testable and visible in one place.
 */
void initializeSystem(SystemContext& context, uint32_t nowMs);

/**
 * Polls physical inputs on their configured schedule and pushes any resulting
 * high-level events into the central queue.
 */
void pollInputs(SystemContext& context, uint32_t nowMs);

/**
 * Consumes queued events and applies the project behavior associated with each.
 */
void processEvents(SystemContext& context, uint32_t nowMs);

/**
 * Advances output subsystems that rely on elapsed time rather than blocking
 * delays, such as blinking LEDs and stepped audio playback.
 */
void updateOutputs(SystemContext& context, uint32_t nowMs);

/**
 * Runs one full iteration of the firmware task pipeline.
 * This function is what `loop()` calls every pass.
 */
void runTasks(SystemContext& context, uint32_t nowMs);
