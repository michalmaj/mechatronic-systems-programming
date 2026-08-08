#include "support/check.hpp"
#include <psm/diverter.hpp>
#include <psm/plant.hpp>

int main() {
    psm::Plant plant;
    psm::Diverter diverter;
    psmCheck(!plant.item.has_value(), "plant starts empty");

    psm::spawnItem(plant, psm::Item{1, psm::Zone::Infeed, 750});
    psmCheck(plant.item.has_value(), "item present after spawnItem");
    psmCheck(plant.item->zone == psm::Zone::Infeed, "spawned item starts at Infeed");

    psm::spawnItem(plant, psm::Item{2, psm::Zone::Infeed, 100});
    psmCheck(plant.item->id == 1, "spawnItem while occupied is ignored");

    psm::advance(plant, diverter);
    psmCheck(plant.item->zone == psm::Zone::PresenceCheck, "advance moves the item forward");

    psm::advance(plant, diverter);
    psm::advance(plant, diverter);
    psmCheck(plant.item->zone == psm::Zone::Diverting, "advance reaches Diverting after three calls");

    return 0;
}
