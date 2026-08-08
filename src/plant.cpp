#include <psm/plant.hpp>

#include <psm/item_motion.hpp>

namespace psm {

void spawnItem(Plant& plant, Item item) {
    if (plant.item.has_value()) {
        return;
    }
    item.zone = Zone::Infeed;
    plant.item = item;
}

void advance(Plant& plant, const Diverter& diverter) {
    if (!plant.item.has_value()) {
        return;
    }
    switch (plant.item->zone) {
        case Zone::Infeed:
        case Zone::PresenceCheck:
        case Zone::Weighing:
            advanceZone(*plant.item);
            return;
        case Zone::Diverting:
            // TODO (Misja 9: przenosnik_czeka_na_dywerter): jeśli !diverter.isSettled(), paczka
            // czeka (nic nie rób -- zostaje w Diverting). Jeśli diverter.isSettled(), skieruj
            // paczkę do OutputLight albo OutputHeavy na podstawie diverter.actualPosition().
            (void)diverter;
            return;
        case Zone::OutputLight:
        case Zone::OutputHeavy:
            plant.item.reset();
            return;
    }
}

}  // namespace psm
