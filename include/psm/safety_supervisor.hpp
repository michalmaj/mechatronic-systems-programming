#pragma once

#include <psm/belt_motor.hpp>
#include <psm/estop_latch.hpp>
#include <psm/mode.hpp>

namespace psm {

struct SafetyDecision {
    bool overrideActive;
};

SafetyDecision checkEmergencyOverride(EStopLatchState latch);

bool diverterMayMove(Mode mode);

BeltMotorCommand filterRoutineBeltCommand(BeltMotorCommand requested, Mode mode);

}  // namespace psm
