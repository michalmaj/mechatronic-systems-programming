#include "support/check.hpp"
#include <psm/engine.hpp>

int main() {
    using psm::Mode;
    using psm::ScenarioInputKind;
    using psm::Zone;

    psm::Engine engine({
        {0, ScenarioInputKind::StartRequested},
        {4, ScenarioInputKind::EmergencyStopPressed},
        {8, ScenarioInputKind::EmergencyStopReleased},
        {9, ScenarioInputKind::Reset},
        {10, ScenarioInputKind::StartRequested},
    });
    engine.spawnItem(psm::Item{1, Zone::Infeed, 750});

    // Ticks 0-1: belt is still ramping up, nothing moves yet.
    auto result = engine.step();
    psmCheck(result.item->zone == Zone::Infeed, "tick 0: belt still Stopped during advance, item waits at Infeed");
    result = engine.step();
    psmCheck(result.item->zone == Zone::Infeed, "tick 1: belt still RampingUp during advance, item waits");

    // Ticks 2-3: belt is Running, the item moves normally.
    result = engine.step();
    psmCheck(result.item->zone == Zone::PresenceCheck, "tick 2: belt Running, item reaches PresenceCheck");
    result = engine.step();
    psmCheck(result.item->zone == Zone::Weighing, "tick 3: item reaches Weighing");

    // Tick 4: e-stop pressed -- the item must NOT reach Diverting this tick.
    result = engine.step();
    psmCheck(result.mode == Mode::EStopped, "tick 4: mode becomes EStopped immediately");
    psmCheck(result.item->zone == Zone::Weighing,
             "tick 4: emergency override stops the belt before Plant::advance, so the item does not move");

    // Tick 5: still EStopped, item stays frozen -- not just a one-tick blip.
    result = engine.step();
    psmCheck(result.mode == Mode::EStopped, "tick 5: still EStopped");
    psmCheck(result.item->zone == Zone::Weighing, "tick 5: item is still frozen at Weighing");

    result = engine.step();  // tick 6, still frozen
    result = engine.step();  // tick 7, still frozen

    // Tick 8: button released. Mode stays EStopped -- Armed still counts as "not Released".
    result = engine.step();
    psmCheck(result.mode == Mode::EStopped,
             "tick 8: releasing the button alone does not clear EStopped (latch is Armed, not Released)");
    psmCheck(result.item->zone == Zone::Weighing, "tick 8: item is still frozen");

    // Tick 9: reset processed. Mode clears to Idle, but motion does NOT resume by itself.
    result = engine.step();
    psmCheck(result.mode == Mode::Idle, "tick 9: Reset clears EStopped to Idle");
    psmCheck(result.item->zone == Zone::Weighing, "tick 9: a reset never restarts motion by itself");

    // Tick 10: start requested again. The belt has to ramp up again before anything moves.
    result = engine.step();
    psmCheck(result.mode == Mode::Running, "tick 10: StartRequested moves Idle -> Running");
    psmCheck(result.item->zone == Zone::Weighing, "tick 10: belt is ramping up again, item has not moved yet");

    result = engine.step();  // tick 11, belt still ramping

    // Tick 12: belt is Running again -- the item finally resumes.
    result = engine.step();
    psmCheck(result.item->zone == Zone::Diverting, "tick 12: item finally resumes moving once the belt is Running again");

    return 0;
}
