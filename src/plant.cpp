#include <psm/plant.hpp>

namespace psm {

bool spawnItem(Plant& plant, ItemId id, Grams mass) {
    if (plant.infeed.has_value()) {
        return false;
    }
    if ((plant.presenceCheck.has_value() && plant.presenceCheck->id == id) ||
        (plant.weighing.has_value() && plant.weighing->id == id) ||
        (plant.diverting.has_value() && plant.diverting->id == id)) {
        return false;
    }
    plant.infeed = Item{id, mass};
    return true;
}

AdvanceResult advance(Plant& plant, const Diverter& diverter, bool routingReady) {
    AdvanceResult result;

    if (plant.diverting.has_value()) {
        if (!routingReady) {
            plant.diverting->divertingWaitTicks = 0;
        } else if (!diverter.isSettled()) {
            ++plant.diverting->divertingWaitTicks;
            result.event = plant.diverting->divertingWaitTicks <= 1
                                ? SystemEventKind::DiverterNotReady
                                : SystemEventKind::RoutingDeadlineMissed;
        } else {
            const Zone destination = diverter.actualPosition() == DiverterPosition::Straight
                                          ? Zone::OutputLight
                                          : Zone::OutputHeavy;
            result.departure = ItemDeparture{plant.diverting->id, destination};
            plant.diverting.reset();
        }
    }

    if (!plant.diverting.has_value() && plant.weighing.has_value()) {
        plant.diverting = std::move(plant.weighing);
        plant.weighing.reset();
        plant.diverting->divertingWaitTicks = 0;
    }

    if (!plant.weighing.has_value() && plant.presenceCheck.has_value()) {
        plant.weighing = std::move(plant.presenceCheck);
        plant.presenceCheck.reset();
    }

    if (!plant.presenceCheck.has_value() && plant.infeed.has_value()) {
        plant.presenceCheck = std::move(plant.infeed);
        plant.infeed.reset();
    }

    return result;
}

}  // namespace psm
