#include "support/check.hpp"
#include <psm/mode.hpp>

int main() {
    using psm::EStopLatchState;
    using psm::Mode;
    using psm::modeStep;

    psmCheck(modeStep(Mode::Running, false, false, EStopLatchState::Engaged) == Mode::EStopped,
             "Engaged latch forces EStopped even while Running");

    psmCheck(modeStep(Mode::Idle, false, false, EStopLatchState::Engaged) == Mode::EStopped,
             "Engaged latch forces EStopped even while Idle");

    psmCheck(modeStep(Mode::Running, false, false, EStopLatchState::Armed) == Mode::EStopped,
             "Armed latch (not yet Released) still forces EStopped");

    psmCheck(modeStep(Mode::EStopped, false, false, EStopLatchState::Released) == Mode::Idle,
             "recovering once latch reads Released lands in Idle, not Running");

    psmCheck(modeStep(Mode::EStopped, true, false, EStopLatchState::Released) == Mode::Idle,
             "same-tick reset+start still lands in Idle -- EStopped recovery takes priority over startRequested");

    psmCheck(modeStep(Mode::Idle, true, false, EStopLatchState::Released) == Mode::Running,
             "a later, separate startRequested (once already Idle) does resume Running");

    psmCheck(modeStep(Mode::Running, false, false, EStopLatchState::Released) == Mode::Running,
             "Released latch: ordinary Running behavior is unaffected");

    return 0;
}
