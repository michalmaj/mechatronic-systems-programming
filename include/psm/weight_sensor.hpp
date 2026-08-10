#pragma once

#include <optional>

#include <psm/fault_kind.hpp>
#include <psm/item.hpp>
#include <psm/sensor_snapshot.hpp>

namespace psm {

class WeightSensor {
public:
    WeightReading read(const std::optional<Item>& item, std::optional<FaultKind> fault);

private:
    std::optional<Grams> lastKnownMass_;
};

}  // namespace psm
