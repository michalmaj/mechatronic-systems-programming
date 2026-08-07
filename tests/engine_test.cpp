#include "support/check.hpp"
#include <psm/engine.hpp>

int main() {
    psm::Engine engine;
    engine.spawnItem(psm::Item{1, psm::Zone::Infeed, 750});

    auto result = engine.step();
    psmCheck(result.tick == 0, "first step reports tick 0");
    psmCheck(result.item.has_value() && result.item->zone == psm::Zone::PresenceCheck,
             "tick 0 moves the item to PresenceCheck");

    result = engine.step();
    psmCheck(result.tick == 1, "second step reports tick 1");
    psmCheck(result.item->zone == psm::Zone::Weighing, "tick 1 moves the item to Weighing");

    result = engine.step();
    psmCheck(result.tick == 2, "third step reports tick 2");
    psmCheck(result.item->zone == psm::Zone::Diverting, "tick 2 moves the item to Diverting");

    result = engine.step();
    psmCheck(result.tick == 3, "fourth step reports tick 3");
    psmCheck(result.item->zone == psm::Zone::OutputHeavy,
             "by tick 3 the diverter has settled and the 750g item routes to OutputHeavy");

    result = engine.step();
    psmCheck(!result.item.has_value(), "tick 4 clears the item");

    result = engine.step();
    psmCheck(result.tick == 5, "sixth step reports tick 5");
    psmCheck(!result.item.has_value(), "tick 5 stays empty");

    return 0;
}
