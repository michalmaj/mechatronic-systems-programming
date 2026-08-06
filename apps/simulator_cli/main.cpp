#include <iostream>

#include <psm/controller.hpp>
#include <psm/plant.hpp>

int main() {
    psm::Plant plant;
    plant.spawnItem(psm::Item{1, psm::Zone::Infeed, 750});

    for (int tick = 0; tick < 6; ++tick) {
        auto diverter = psm::DiverterPosition::Straight;
        if (const auto& item = plant.currentItem(); item.has_value()) {
            diverter = psm::toDiverterPosition(psm::classify(item->mass));
        }

        plant.advance(diverter);

        if (const auto& item = plant.currentItem(); item.has_value()) {
            std::cout << "tick " << tick << ": item " << item->id << " in zone "
                      << psm::toString(item->zone) << '\n';
        } else {
            std::cout << "tick " << tick << ": empty\n";
        }
    }

    return 0;
}
