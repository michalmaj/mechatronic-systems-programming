#pragma once

#include <psm/estop_latch.hpp>

namespace psm {

enum class Mode { Idle, Running, EStopped };

Mode modeStep(Mode current, bool startRequested, bool stopRequested,
              EStopLatchState latch = EStopLatchState::Released);

}  // namespace psm
