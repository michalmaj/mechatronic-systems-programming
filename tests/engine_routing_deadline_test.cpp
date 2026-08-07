#include "support/check.hpp"
#include <psm/engine.hpp>

int main() {
    using psm::Mode;
    using psm::ScenarioInputKind;
    using psm::SystemEventKind;
    using psm::Zone;

    psm::Engine engine(
        {{0, ScenarioInputKind::StartRequested},
         {9, ScenarioInputKind::Reset},
         {10, ScenarioInputKind::StartRequested}},
        {{0, 7, psm::FaultTarget::Diverter, psm::FaultKind::Blocked}});
    engine.spawnItem(psm::Item{1, Zone::Infeed, 750});

    for (int i = 0; i < 4; ++i) {
        engine.step();  // ticks 0-3: normal ramp-up and travel to Weighing
    }

    auto result = engine.step();  // tick 4
    psmCheck(result.item->zone == Zone::Diverting, "tick 4: item reaches Diverting; diverter is still blocked");

    result = engine.step();  // tick 5
    psmCheck(result.event == SystemEventKind::DiverterNotReady,
             "tick 5: first blocked tick at Diverting is DiverterNotReady, not yet a deadline miss");
    psmCheck(result.mode == Mode::Running, "tick 5: mode is still Running");

    result = engine.step();  // tick 6
    psmCheck(result.event == SystemEventKind::RoutingDeadlineMissed,
             "tick 6: a second consecutive blocked tick exceeds the deadline");
    psmCheck(result.mode == Mode::Fault, "tick 6: mode transitions Running -> Fault");
    psmCheck(result.item->zone == Zone::Diverting, "tick 6: item is stuck, not routed anywhere");

    engine.step();  // tick 7 -- the Blocked fault window has just ended, but Mode doesn't know that
    result = engine.step();  // tick 8
    psmCheck(result.mode == Mode::Fault,
             "tick 8: still Fault -- the underlying fault clearing does not self-heal Mode");
    psmCheck(result.item->zone == Zone::Diverting, "tick 8: item still stuck -- diverter is untouched while not Running");

    result = engine.step();  // tick 9: Reset
    psmCheck(result.mode == Mode::Idle, "tick 9: Reset clears Fault to Idle");
    psmCheck(result.item->zone == Zone::Diverting, "tick 9: a reset never resumes motion by itself");

    result = engine.step();  // tick 10: StartRequested
    psmCheck(result.mode == Mode::Running, "tick 10: StartRequested resumes Running");
    psmCheck(result.item->zone == Zone::Diverting, "tick 10: belt has to ramp up again before anything moves");

    engine.step();  // tick 11
    result = engine.step();  // tick 12
    psmCheck(result.item->zone == Zone::OutputHeavy,
             "tick 12: once the now-unblocked diverter finally settles, the item routes correctly");

    result = engine.step();  // tick 13
    psmCheck(!result.item.has_value(), "tick 13: item clears -- the whole system recovers from the fault");

    return 0;
}
