#pragma once

#include <psm/estop_latch.hpp>
#include <psm/mode.hpp>

namespace psm {

struct SafetyDecision {
    bool overrideActive;
};

SafetyDecision checkEmergencyOverride(EStopLatchState latch);

bool diverterMayMove(Mode mode);

}  // namespace psm
