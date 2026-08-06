#include "support/check.hpp"
#include <psm/plant.hpp>

int main() {
    psm::Plant plant;
    psmCheck(!plant.currentItem().has_value(), "plant starts empty");

    plant.spawnItem(psm::Item{1, psm::Zone::Infeed, 750});
    psmCheck(plant.currentItem().has_value(), "item present after spawn");
    psmCheck(plant.currentItem()->zone == psm::Zone::Infeed, "spawned item starts at Infeed");

    plant.spawnItem(psm::Item{2, psm::Zone::Infeed, 100});
    psmCheck(plant.currentItem()->id == 1, "spawning while occupied is ignored in R0");

    plant.advance(psm::DiverterPosition::Straight);
    psmCheck(plant.currentItem()->zone == psm::Zone::PresenceCheck, "advances to PresenceCheck");

    plant.advance(psm::DiverterPosition::Straight);
    psmCheck(plant.currentItem()->zone == psm::Zone::Weighing, "advances to Weighing");

    plant.advance(psm::DiverterPosition::Straight);
    psmCheck(plant.currentItem()->zone == psm::Zone::Diverting, "advances to Diverting");

    plant.advance(psm::DiverterPosition::Diverted);
    psmCheck(plant.currentItem()->zone == psm::Zone::OutputHeavy, "Diverted routes to OutputHeavy");

    plant.advance(psm::DiverterPosition::Straight);
    psmCheck(!plant.currentItem().has_value(), "item clears one tick after reaching an output zone");

    return 0;
}
