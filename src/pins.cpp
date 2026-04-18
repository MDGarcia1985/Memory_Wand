/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#include "pins.h"

namespace pins {

const PinAssignments& getPinAssignments()
{
    // This single table is the hardware map for the active board definition.
    // If the PCB changes, this should be the main place that changes with it.
    static const PinAssignments kPins = {3, 4, 5, 6, 7, -1};

    return kPins;
}

}  // namespace pins
