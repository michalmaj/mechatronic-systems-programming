#include <iostream>

#include <psm/controller.hpp>
#include <psm/diverter.hpp>
#include <psm/plant.hpp>
#include <psm/zone.hpp>

int main() {
    psm::Plant plant;
    psm::Diverter diverter;
    psm::spawnItem(plant, psm::Item{1, psm::Zone::Infeed, 750});

    for (int tick = 0; tick < 8; ++tick) {
        if (plant.item.has_value()) {
            diverter.setCommand(psm::toDiverterCommand(psm::classify(plant.item->mass)));
        }
        diverter.resolve();
        psm::advance(plant, diverter);

        if (plant.item.has_value()) {
            std::cout << "tick " << tick << ": zone=" << psm::toString(plant.item->zone) << '\n';
        } else {
            std::cout << "tick " << tick << ": empty\n";
        }
    }

    return 0;
}
