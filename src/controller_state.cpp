#include <psm/controller_state.hpp>

#include <psm/controller.hpp>
#include <psm/zone.hpp>

namespace psm {

void updateControllerState(ControllerState& state, const std::optional<Item>& item, PresenceReading presence,
                            WeightReading weight) {
    // TODO (Misja 23: pamiec_decyzji_sterownika):
    // 1) jeśli !item.has_value() albo item->zone == Zone::Infeed: state = ControllerState{} (reset).
    // 2) w przeciwnym razie, jeśli item->zone == Zone::PresenceCheck i presence.status==Ok i
    //    presence.occupied: state.presenceConfirmed = true.
    // 3) w przeciwnym razie, jeśli item->zone == Zone::Weighing i state.presenceConfirmed i
    //    weight.status==Ok: state.classification = decideClassification(weight).
    // 4) w przeciwnym razie: bez zmian.
    (void)item;
    (void)presence;
    (void)weight;
}

}  // namespace psm
