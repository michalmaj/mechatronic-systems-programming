#pragma once

#include <string_view>

#include <psm/estop_latch.hpp>

namespace psm {

enum class Mode { Idle, Running, Fault, EStopped, Maintenance };

struct ModeInputs {
    EStopLatchState estop;
    bool startRequested;
    bool stopRequested;
    bool resetRequested;
};

Mode modeStep(Mode previous, ModeInputs inputs);
std::string_view toString(Mode mode);

}  // namespace psm
