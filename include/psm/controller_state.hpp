#pragma once

#include <optional>

#include <psm/item.hpp>
#include <psm/sensor_snapshot.hpp>
#include <psm/weight_class.hpp>

namespace psm {

struct ControllerState {
    bool presenceConfirmed = false;
    std::optional<WeightClass> classification;
};

void updateControllerState(ControllerState& state, const std::optional<Item>& item, PresenceReading presence,
                            WeightReading weight);

}  // namespace psm
