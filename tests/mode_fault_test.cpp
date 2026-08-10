#include "support/check.hpp"
#include <psm/mode.hpp>
#include <psm/safety_supervisor.hpp>
#include <psm/system_event_kind.hpp>

int main() {
    using psm::EStopLatchState;
    using psm::Mode;
    using psm::modeStep;
    using psm::reactToSystemEvent;
    using psm::SystemEventKind;

    // -- reactToSystemEvent: the whole rule.
    psmCheck(reactToSystemEvent(Mode::Running, SystemEventKind::RoutingDeadlineMissed) == Mode::Fault,
             "Running + RoutingDeadlineMissed -> Fault");
    psmCheck(reactToSystemEvent(Mode::Running, SystemEventKind::DiverterNotReady) == Mode::Running,
             "Running + DiverterNotReady (not yet a deadline miss) -> unchanged");
    psmCheck(reactToSystemEvent(Mode::Running, std::nullopt) == Mode::Running,
             "Running + no event -> unchanged");
    psmCheck(reactToSystemEvent(Mode::Idle, SystemEventKind::RoutingDeadlineMissed) == Mode::Idle,
             "RoutingDeadlineMissed only matters coming from Running -- Idle is unaffected");
    psmCheck(reactToSystemEvent(Mode::Fault, SystemEventKind::RoutingDeadlineMissed) == Mode::Fault,
             "already Fault -- reactToSystemEvent cannot re-trigger or change it");

    // -- modeStep: Fault is latching, and neither stopRequested nor startRequested can leave it.
    psmCheck(modeStep(Mode::Fault, false, false) == Mode::Fault, "Fault persists with no input at all");
    psmCheck(modeStep(Mode::Fault, false, true) == Mode::Fault,
             "stopRequested cannot leave Fault (it's already effectively stopped)");
    psmCheck(modeStep(Mode::Fault, true, false) == Mode::Fault, "startRequested cannot leave Fault either");
    psmCheck(modeStep(Mode::Fault, true, true) == Mode::Fault, "neither can both at once");
    psmCheck(modeStep(Mode::Fault, false, false, EStopLatchState::Released, true) == Mode::Idle,
             "only an explicit resetRequested clears Fault, and it clears to Idle, never straight to Running");
    psmCheck(modeStep(Mode::Fault, true, false, EStopLatchState::Released, true) == Mode::Idle,
             "same-tick reset+start still lands in Idle -- recovery always goes through Idle first, "
             "same pattern as e-stop recovery");

    // -- e-stop still has absolute priority over Fault.
    psmCheck(modeStep(Mode::Fault, false, false, EStopLatchState::Engaged) == Mode::EStopped,
             "an e-stop press overrides Fault just like it overrides every other mode");

    // -- diverterMayMove already correctly generalizes to the new Mode value, no code change needed.
    psmCheck(!psm::diverterMayMove(Mode::Fault), "diverter may not move while Fault, same as Idle/EStopped");

    return 0;
}
