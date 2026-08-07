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

std::string_view toString(Mode mode) {
    switch (mode) {
        case Mode::Idle: return "Idle";
        case Mode::Running: return "Running";
        case Mode::Fault: return "Fault";
        case Mode::EStopped: return "EStopped";
        case Mode::Maintenance: return "Maintenance";
    }
    return "Unknown";
}

}  // namespace psm
