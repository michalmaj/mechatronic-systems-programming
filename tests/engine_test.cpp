#include "support/check.hpp"
#include <psm/engine.hpp>

int main() {
    psm::Engine engine({{0, psm::ScenarioInputKind::StartRequested}});
    engine.spawnItem(psm::Item{1, psm::Zone::Infeed, 750});

    auto result = engine.step();
    psmCheck(result.tick == 0, "first step reports tick 0");
    psmCheck(result.item.has_value() && result.item->zone == psm::Zone::Infeed,
             "tick 0: belt is still Stopped during advance, item waits at Infeed");

    result = engine.step();
    psmCheck(result.tick == 1, "second step reports tick 1");
    psmCheck(result.item->zone == psm::Zone::Infeed,
             "tick 1: belt is still RampingUp during advance, item waits");

    result = engine.step();
    psmCheck(result.tick == 2, "third step reports tick 2");
    psmCheck(result.item->zone == psm::Zone::PresenceCheck, "tick 2: belt is Running, item reaches PresenceCheck");

    result = engine.step();
    psmCheck(result.item->zone == psm::Zone::Weighing, "tick 3: item reaches Weighing");

    result = engine.step();
    psmCheck(result.item->zone == psm::Zone::Diverting, "tick 4: item reaches Diverting");

    result = engine.step();
    psmCheck(result.item->zone == psm::Zone::OutputHeavy,
             "tick 5: the diverter has settled and the 750g item routes to OutputHeavy");

    result = engine.step();
    psmCheck(!result.item.has_value(), "tick 6: item clears");

    result = engine.step();
    psmCheck(result.tick == 7, "eighth step reports tick 7");
    psmCheck(!result.item.has_value(), "tick 7: stays empty");

    return 0;
}
