#include <psm/mode.hpp>

namespace psm {

Mode modeStep(Mode current, bool startRequested, bool stopRequested, EStopLatchState latch, bool resetRequested) {
    if (latch != EStopLatchState::Released) {
        return Mode::EStopped;
    }
    if (current == Mode::EStopped) {
        return Mode::Idle;
    }
    if (current == Mode::Fault) {
        return resetRequested ? Mode::Idle : Mode::Fault;
    }
    if (stopRequested) {
        return Mode::Idle;
    }
    if (startRequested) {
        return Mode::Running;
    }
    return current;
}

Mode reactToSystemEvent(Mode modeForTick, std::optional<SystemEventKind> event) {
    if (modeForTick == Mode::Running && event.has_value() && *event == SystemEventKind::RoutingDeadlineMissed) {
        return Mode::Fault;
    }
    return modeForTick;
}

}  // namespace psm
