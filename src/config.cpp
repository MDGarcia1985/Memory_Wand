/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#include "config.h"

namespace config {

const SystemConfig& getSystemConfig()
{
    static const SystemConfig kConfig = {
        {5, 800, true, true, 6, 4, 2},
        {120, 120},
    };

    return kConfig;
}

}  // namespace config
