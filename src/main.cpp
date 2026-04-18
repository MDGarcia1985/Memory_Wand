/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#include <Arduino.h>

#include "headers.h"

void setup()
{
    // The project keeps setup thin and delegates the actual subsystem bring-up
    // to the task layer so the initialization order stays centralized.
    runTasks(systemContext(), millis());
}

void loop()
{
    // Every pass runs the same non-blocking pipeline:
    // initialize if needed -> poll inputs -> process events -> update outputs.
    runTasks(systemContext(), millis());
}
