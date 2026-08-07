#include <psm/mode.hpp>

namespace psm {

Mode modeStep(Mode previous, ModeInputs inputs) {
    if (inputs.estop != EStopLatchState::Released) {
        return Mode::EStopped;
    }
    if (previous == Mode::EStopped) {
        return Mode::Idle;
    }
    if (previous == Mode::Fault && inputs.resetRequested) {
        return Mode::Idle;
    }
    if (previous == Mode::Idle && inputs.startRequested) {
        return Mode::Running;
    }
    if (previous == Mode::Running && inputs.stopRequested) {
        return Mode::Idle;
    }
    return previous;
}

}  // namespace psm
