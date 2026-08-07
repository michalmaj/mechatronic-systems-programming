#pragma once

#include <optional>

#include <psm/item.hpp>
#include <psm/scripted_fault.hpp>
#include <psm/sensor_snapshot.hpp>

namespace psm {

class PresenceSensor {
public:
    PresenceReading read(const std::optional<Item>& item, std::optional<FaultKind> fault);

private:
    bool lastKnownOccupied_ = false;
};

}  // namespace psm
