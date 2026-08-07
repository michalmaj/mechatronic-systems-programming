#include <iostream>

#include <psm/engine.hpp>

int main() {
    psm::Engine engine;
    engine.spawnItem(psm::Item{1, psm::Zone::Infeed, 750});

    for (int i = 0; i < 6; ++i) {
        const psm::TickResult result = engine.step();

        if (result.item.has_value()) {
            std::cout << "tick " << result.tick << ": item " << result.item->id << " in zone "
                      << psm::toString(result.item->zone) << '\n';
        } else {
            std::cout << "tick " << result.tick << ": empty\n";
        }
    }

    return 0;
}
