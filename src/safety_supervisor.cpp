#include <psm/safety_supervisor.hpp>

namespace psm {

SafetyDecision checkEmergencyOverride(EStopLatchState latch) {
    return SafetyDecision{latch != EStopLatchState::Released};
}

bool diverterMayMove(Mode mode) {
    return mode == Mode::Running;
}

BeltMotorCommand filterRoutineBeltCommand(BeltMotorCommand requested, Mode mode) {
    return mode == Mode::Running ? requested : BeltMotorCommand::Stop;
}

}  // namespace psm
