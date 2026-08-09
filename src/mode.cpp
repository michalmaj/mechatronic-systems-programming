#include <psm/mode.hpp>

namespace psm {

Mode modeStep(Mode current, bool startRequested, bool stopRequested) {
    if (stopRequested) {
        return Mode::Idle;
    }
    if (startRequested) {
        return Mode::Running;
    }
    return current;
}

}  // namespace psm
