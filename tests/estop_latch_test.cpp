#include "support/check.hpp"
#include <psm/estop_latch.hpp>

int main() {
    using psm::EStopLatchState;
    using psm::nextEStopLatchState;

    psmCheck(nextEStopLatchState(EStopLatchState::Released, false, false, false) == EStopLatchState::Released,
             "stays Released with no input");

    psmCheck(nextEStopLatchState(EStopLatchState::Released, true, false, false) == EStopLatchState::Engaged,
             "Released -> Engaged on Pressed");

    psmCheck(nextEStopLatchState(EStopLatchState::Engaged, false, false, true) == EStopLatchState::Engaged,
             "Reset is ignored while still Engaged (pressed)");

    psmCheck(nextEStopLatchState(EStopLatchState::Engaged, false, true, false) == EStopLatchState::Armed,
             "Engaged -> Armed on Released");

    psmCheck(nextEStopLatchState(EStopLatchState::Armed, false, false, false) == EStopLatchState::Armed,
             "stays Armed until Reset");

    psmCheck(nextEStopLatchState(EStopLatchState::Armed, false, false, true) == EStopLatchState::Released,
             "Armed -> Released on Reset");

    psmCheck(nextEStopLatchState(EStopLatchState::Armed, true, false, true) == EStopLatchState::Engaged,
             "Pressed always wins even alongside Reset");

    return 0;
}
