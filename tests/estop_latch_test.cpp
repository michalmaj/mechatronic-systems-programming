#include "support/check.hpp"
#include <psm/estop_latch.hpp>

int main() {
    using psm::EStopLatchState;
    using psm::nextEStopLatchState;

    psmCheck(nextEStopLatchState(EStopLatchState::Released, false, false, false) == EStopLatchState::Released,
             "stays Released with no input");

    psmCheck(nextEStopLatchState(EStopLatchState::Released, true, false, false) == EStopLatchState::Engaged,
             "Released -> Engaged on pressed");

    psmCheck(nextEStopLatchState(EStopLatchState::Engaged, false, true, false) == EStopLatchState::Armed,
             "Engaged -> Armed on released");

    psmCheck(nextEStopLatchState(EStopLatchState::Engaged, false, false, true) == EStopLatchState::Engaged,
             "resetRequested alone has no effect while still Engaged");

    psmCheck(nextEStopLatchState(EStopLatchState::Engaged, false, true, true) == EStopLatchState::Armed,
             "released+resetRequested together: only released takes effect, result is Armed not Released");

    psmCheck(nextEStopLatchState(EStopLatchState::Armed, false, false, false) == EStopLatchState::Armed,
             "stays Armed until resetRequested");

    psmCheck(nextEStopLatchState(EStopLatchState::Armed, false, false, true) == EStopLatchState::Released,
             "Armed -> Released on resetRequested");

    psmCheck(nextEStopLatchState(EStopLatchState::Armed, true, false, true) == EStopLatchState::Engaged,
             "pressed always wins even alongside resetRequested");

    return 0;
}
