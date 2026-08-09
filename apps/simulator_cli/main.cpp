#include <iostream>

#include <psm/engine.hpp>
#include <psm/item.hpp>
#include <psm/tick_result.hpp>
#include <psm/zone.hpp>

int main() {
    psm::Engine engine;
    engine.spawnItem(psm::Item{1, psm::Zone::Infeed, 750});

    for (int i = 0; i < 8; ++i) {
        const psm::TickResult result = engine.step();
        std::cout << psm::describe(result) << '\n';
    }

    return 0;
}
