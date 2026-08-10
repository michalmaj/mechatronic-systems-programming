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

std::optional<SystemEventKind> advance(Plant& plant, const Diverter& diverter, bool routingReady) {
    if (!plant.item.has_value()) {
        return std::nullopt;
    }
    switch (plant.item->zone) {
        case Zone::Infeed:
        case Zone::PresenceCheck:
            advanceZone(*plant.item);
            return std::nullopt;
        case Zone::Weighing:
            advanceZone(*plant.item);
            plant.divertingWaitTicks = 0;
            return std::nullopt;
        case Zone::Diverting:
            // TODO (Misja 26: termin_rutowania): !routingReady musi zerować divertingWaitTicks
            // (nie zamrażać go) i zwracać nullopt; routingReady && !isSettled() zwiększa licznik i
            // zwraca DiverterNotReady (licznik <= 1) albo RoutingDeadlineMissed (licznik > 1);
            // settled rutuje normalnie i zeruje licznik.
            if (!routingReady || !diverter.isSettled()) {
                return std::nullopt;
            }
            plant.item->zone =
                (diverter.actualPosition() == DiverterPosition::Straight) ? Zone::OutputLight : Zone::OutputHeavy;
            return std::nullopt;
        case Zone::OutputLight:
        case Zone::OutputHeavy:
            plant.item.reset();
            return std::nullopt;
    }
    return std::nullopt;
}

}  // namespace psm
