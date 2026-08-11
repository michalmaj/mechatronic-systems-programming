#include "support/check.hpp"
#include <psm/engine.hpp>

int main() {
    psm::Engine engine;

    auto result = engine.step();
    psmCheck(result.tick == 0, "first step reports tick 0");

    result = engine.step();
    psmCheck(result.tick == 1, "tick counter increments by exactly one each call");

    result = engine.step();
    psmCheck(result.tick == 2, "tick counter keeps incrementing");

    psmCheck(engine.spawnItem(1, 750), "spawning into a fresh engine succeeds");
    engine.requestStart();

    result = engine.step();
    result = engine.step();
    psmCheck(result.presenceCheck.has_value() && result.presenceCheck->id == 1,
             "once running, a spawned parcel actually advances through the plant");

    return 0;
}
