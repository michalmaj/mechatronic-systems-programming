#include "support/check.hpp"
#include <psm/plant.hpp>

int main() {
    psm::Plant plant;
    psmCheck(!plant.infeed.has_value(), "plant starts with an empty infeed");
    psmCheck(!plant.presenceCheck.has_value(), "plant starts with an empty presenceCheck");
    psmCheck(!plant.weighing.has_value(), "plant starts with an empty weighing");
    psmCheck(!plant.diverting.has_value(), "plant starts with an empty diverting");

    psmCheck(psm::spawnItem(plant, 1, 750), "spawnItem succeeds into an empty infeed");
    psmCheck(plant.infeed.has_value() && plant.infeed->id == 1 && plant.infeed->mass == 750,
             "spawned item lands in infeed with the given id and mass");

    psmCheck(!psm::spawnItem(plant, 2, 100), "spawnItem fails while infeed is still occupied");
    psmCheck(plant.infeed->id == 1, "the occupying item is unchanged after the rejected spawn");

    plant.presenceCheck = plant.infeed;
    plant.infeed.reset();
    psmCheck(!psm::spawnItem(plant, 1, 500),
             "spawnItem fails when id collides with a parcel resident elsewhere in the plant");

    psmCheck(psm::spawnItem(plant, 2, 500),
             "a distinct id spawns successfully even while another parcel is resident");
    psmCheck(plant.infeed.has_value() && plant.infeed->id == 2, "the new parcel lands in infeed");

    return 0;
}
