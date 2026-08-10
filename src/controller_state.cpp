#include <psm/controller_state.hpp>

#include <psm/controller.hpp>
#include <psm/zone.hpp>

namespace psm {

void updateControllerState(ControllerState& state, const std::optional<Item>& item, PresenceReading presence,
                            WeightReading weight) {
    if (!item.has_value() || item->zone == Zone::Infeed) {
        state = ControllerState{};
        return;
    }
    if (item->zone == Zone::PresenceCheck && presence.status == ReadingStatus::Ok && presence.occupied) {
        state.presenceConfirmed = true;
        return;
    }
    if (item->zone == Zone::Weighing && state.presenceConfirmed && weight.status == ReadingStatus::Ok) {
        state.classification = decideClassification(weight);
        return;
    }
}

}  // namespace psm
