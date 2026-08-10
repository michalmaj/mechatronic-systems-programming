#include <psm/mode.hpp>

namespace psm {

Mode modeStep(Mode current, bool startRequested, bool stopRequested, EStopLatchState latch) {
    if (latch != EStopLatchState::Released) {
        return Mode::EStopped;
    }
    if (current == Mode::EStopped) {
        return Mode::Idle;
    }
    if (stopRequested) {
        return Mode::Idle;
    }
    if (startRequested) {
        return Mode::Running;
    }
    return current;
}

}  // namespace psm
