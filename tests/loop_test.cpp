#include "support/check.hpp"
#include <psm/loop.hpp>

int main() {
    psm::Plant plant;
    psm::spawnItem(plant, psm::Item{1, psm::Zone::Infeed, 750});

    psm::runTicks(plant, 4);
    psmCheck(plant.item.has_value() && plant.item->zone == psm::Zone::OutputHeavy,
             "after 4 ticks a 750g parcel has reached OutputHeavy");

    psm::runTicks(plant, 1);
    psmCheck(!plant.item.has_value(), "one more tick clears the item from the system");

    return 0;
}
