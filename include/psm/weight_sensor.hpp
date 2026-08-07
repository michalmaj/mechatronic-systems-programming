#pragma once

#include <optional>

#include <psm/item.hpp>
#include <psm/scripted_fault.hpp>
#include <psm/sensor_snapshot.hpp>

namespace psm {

class WeightSensor {
public:
    WeightReading read(const std::optional<Item>& item, std::optional<FaultKind> fault);

private:
    Grams lastKnownGrams_ = 0;
};

}  // namespace psm
