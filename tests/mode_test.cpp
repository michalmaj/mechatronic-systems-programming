#include "support/check.hpp"
#include <psm/mode.hpp>

int main() {
    using psm::EStopLatchState;
    using psm::Mode;
    using psm::ModeInputs;
    using psm::modeStep;

    psmCheck(modeStep(Mode::Idle, ModeInputs{EStopLatchState::Released, true, false, false}) == Mode::Running,
             "Idle -> Running on startRequested");

    psmCheck(modeStep(Mode::Running, ModeInputs{EStopLatchState::Released, false, true, false}) == Mode::Idle,
             "Running -> Idle on stopRequested");

    psmCheck(modeStep(Mode::Running, ModeInputs{EStopLatchState::Released, false, false, false}) == Mode::Running,
             "Running stays Running with no input");

    psmCheck(modeStep(Mode::Running, ModeInputs{EStopLatchState::Engaged, false, false, false}) == Mode::EStopped,
             "Running -> EStopped when latch Engaged");

    psmCheck(modeStep(Mode::Idle, ModeInputs{EStopLatchState::Armed, false, false, false}) == Mode::EStopped,
             "Idle -> EStopped when latch Armed");

    psmCheck(modeStep(Mode::EStopped, ModeInputs{EStopLatchState::Released, false, false, false}) == Mode::Idle,
             "EStopped -> Idle once latch Released");

    psmCheck(modeStep(Mode::Fault, ModeInputs{EStopLatchState::Released, false, false, false}) == Mode::Fault,
             "Fault stays Fault without resetRequested");

    psmCheck(modeStep(Mode::Fault, ModeInputs{EStopLatchState::Released, false, false, true}) == Mode::Idle,
             "Fault -> Idle on resetRequested");

    psmCheck(modeStep(Mode::Maintenance, ModeInputs{EStopLatchState::Released, true, true, true}) == Mode::Maintenance,
             "Maintenance is unaffected by any input (no transition into or out of it exists yet)");

    psmCheck(modeStep(Mode::Running, ModeInputs{EStopLatchState::Released, false, false, false, true}) == Mode::Fault,
             "Running -> Fault when routingDeadlineMissed");

    psmCheck(modeStep(Mode::Idle, ModeInputs{EStopLatchState::Released, false, false, false, true}) == Mode::Idle,
             "routingDeadlineMissed has no effect outside Running -- nothing is in flight to miss a deadline on");

    psmCheck(modeStep(Mode::Fault, ModeInputs{EStopLatchState::Released, false, false, true, false}) == Mode::Idle,
             "Fault still clears to Idle via resetRequested, unaffected by the new field");

    return 0;
}
