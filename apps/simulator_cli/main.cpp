#include <iostream>

#include <psm/engine.hpp>

int main() {
    psm::Engine engine({{0, psm::ScenarioInputKind::StartRequested}},
                        {{0, 4, psm::FaultTarget::WeightSensor, psm::FaultKind::Missing}});
    engine.spawnItem(psm::Item{1, psm::Zone::Infeed, 750});

    for (int i = 0; i < 7; ++i) {
        const psm::TickResult result = engine.step();

        std::cout << "tick " << result.tick << ": mode=" << psm::toString(result.mode)
                   << ", weight=" << psm::toString(result.sensors.weight.status) << ", ";
        if (result.item.has_value()) {
            std::cout << "item " << result.item->id << " in zone " << psm::toString(result.item->zone) << '\n';
        } else {
            std::cout << "empty\n";
        }
    }

    return 0;
}
