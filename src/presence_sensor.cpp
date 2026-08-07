#include <psm/presence_sensor.hpp>

namespace psm {

PresenceReading PresenceSensor::read(const std::optional<Item>& item, std::optional<FaultKind> fault) {
    const bool groundTruth = item.has_value();

    if (!fault.has_value()) {
        lastKnownOccupied_ = groundTruth;
        return PresenceReading{ReadingStatus::Ok, groundTruth};
    }
    if (*fault == FaultKind::Missing) {
        return PresenceReading{ReadingStatus::Missing, false};
    }
    // FaultKind::Stale is the only other kind isValidFaultScript allows for PresenceSensor.
    return PresenceReading{ReadingStatus::Stale, lastKnownOccupied_};
}

}  // namespace psm
