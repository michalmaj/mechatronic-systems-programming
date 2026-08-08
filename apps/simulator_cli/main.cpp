#include <iostream>

#include <psm/controller.hpp>
#include <psm/plant.hpp>
#include <psm/zone.hpp>

int main() {
    psm::Plant plant;
    psm::spawnItem(plant, psm::Item{1, psm::Zone::Infeed, 750});

    for (int tick = 0; tick < 6; ++tick) {
        psm::DiverterPosition diverterPosition = psm::DiverterPosition::Straight;
        if (plant.item.has_value()) {
            diverterPosition = psm::toDiverterPosition(psm::classify(plant.item->mass));
        }
        psm::advance(plant, diverterPosition);

        if (plant.item.has_value()) {
            std::cout << "tick " << tick << ": zone=" << psm::toString(plant.item->zone) << '\n';
        } else {
            std::cout << "tick " << tick << ": empty\n";
        }
    }

    return 0;
}
