#pragma once

#include <optional>

#include <psm/fault_kind.hpp>
#include <psm/item.hpp>
#include <psm/sensor_snapshot.hpp>

namespace psm {

class PresenceSensor {
public:
    PresenceReading read(const std::optional<Item>& item, std::optional<FaultKind> fault);

private:
    std::optional<bool> lastKnownOccupied_;
};

}  // namespace psm
