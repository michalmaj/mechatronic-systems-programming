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

void advance(Plant& plant, DiverterPosition diverterPosition) {
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
            plant.item->zone =
                (diverterPosition == DiverterPosition::Straight) ? Zone::OutputLight : Zone::OutputHeavy;
            return;
        case Zone::OutputLight:
        case Zone::OutputHeavy:
            plant.item.reset();
            return;
    }
}

}  // namespace psm
