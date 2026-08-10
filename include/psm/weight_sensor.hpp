#pragma once

#include <optional>

#include <psm/item.hpp>
#include <psm/sensor_fault_kind.hpp>
#include <psm/sensor_snapshot.hpp>

namespace psm {

class WeightSensor {
public:
    WeightReading read(const std::optional<Item>& item, std::optional<SensorFaultKind> fault);

private:
    std::optional<Grams> lastKnownMass_;
};

}  // namespace psm
