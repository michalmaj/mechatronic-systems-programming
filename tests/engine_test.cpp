#include "support/check.hpp"
#include <psm/engine.hpp>

int main() {
    psm::Engine engine;

    auto result = engine.step();
    psmCheck(result.tick == 0, "first step reports tick 0");
    psmCheck(!result.item.has_value(), "an engine with nothing spawned reports no item");

    result = engine.step();
    psmCheck(result.tick == 1, "tick counter increments by exactly one each call");

    result = engine.step();
    psmCheck(result.tick == 2, "tick counter keeps incrementing");

    return 0;
}
