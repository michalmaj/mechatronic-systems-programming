#include <iostream>

#include <psm/engine.hpp>

int main() {
    psm::Engine engine({
        {0, psm::ScenarioInputKind::StartRequested},
        {4, psm::ScenarioInputKind::EmergencyStopPressed},
        {8, psm::ScenarioInputKind::EmergencyStopReleased},
        {9, psm::ScenarioInputKind::Reset},
        {10, psm::ScenarioInputKind::StartRequested},
    });
    engine.spawnItem(psm::Item{1, psm::Zone::Infeed, 750});

    for (int i = 0; i < 15; ++i) {
        const psm::TickResult result = engine.step();

        std::cout << "tick " << result.tick << ": mode=" << psm::toString(result.mode) << ", ";
        if (result.item.has_value()) {
            std::cout << "item " << result.item->id << " in zone " << psm::toString(result.item->zone) << '\n';
        } else {
            std::cout << "empty\n";
        }
    }

    return 0;
}
