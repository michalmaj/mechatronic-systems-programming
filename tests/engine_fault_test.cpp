#include "support/check.hpp"
#include <psm/engine.hpp>

int main() {
    psm::Engine engine({{0, psm::ScenarioInputKind::StartRequested}},
                        {{0, 4, psm::FaultTarget::WeightSensor, psm::FaultKind::Missing}});
    engine.spawnItem(psm::Item{1, psm::Zone::Infeed, 750});

    auto result = engine.step();  // tick 0
    psmCheck(result.sensors.weight.status == psm::ReadingStatus::Missing,
             "tick 0: weight sensor reports Missing due to the active fault");
    psmCheck(result.item->zone == psm::Zone::Infeed, "tick 0: item still at Infeed (belt not running yet)");

    result = engine.step();  // tick 1
    psmCheck(result.item->zone == psm::Zone::Infeed, "tick 1: still Infeed (belt still ramping)");

    result = engine.step();  // tick 2
    psmCheck(result.item->zone == psm::Zone::PresenceCheck, "tick 2: belt Running, item reaches PresenceCheck");

    result = engine.step();  // tick 3
    psmCheck(result.sensors.weight.status == psm::ReadingStatus::Missing, "tick 3: fault still active");
    psmCheck(result.item->zone == psm::Zone::Weighing,
             "tick 3: item reaches Weighing, but classification is still blocked");

    result = engine.step();  // tick 4
    psmCheck(result.sensors.weight.status == psm::ReadingStatus::Ok, "tick 4: fault has cleared");
    psmCheck(result.item->zone == psm::Zone::Diverting,
             "tick 4: item reaches Diverting just as classification finally succeeds");

    result = engine.step();  // tick 5
    psmCheck(result.item->zone == psm::Zone::Diverting,
             "tick 5: item must wait -- the diverter didn't have lead time to settle, because the fault delayed "
             "classification until the same tick the item arrived");

    result = engine.step();  // tick 6
    psmCheck(result.item->zone == psm::Zone::OutputHeavy,
             "tick 6: once the diverter finally settles, the item still routes correctly -- the fault caused a "
             "delay, not a misrouting");

    return 0;
}
