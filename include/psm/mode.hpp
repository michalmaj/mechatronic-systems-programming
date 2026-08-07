#pragma once

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

}  // namespace psm
