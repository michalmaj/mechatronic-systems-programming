#pragma once

#include <optional>

#include <psm/item.hpp>
#include <psm/sensor_fault_kind.hpp>
#include <psm/sensor_snapshot.hpp>

namespace psm {

class PresenceSensor {
public:
    PresenceReading read(const std::optional<Item>& item, std::optional<SensorFaultKind> fault);

private:
    std::optional<bool> lastKnownOccupied_;
};

}  // namespace psm
