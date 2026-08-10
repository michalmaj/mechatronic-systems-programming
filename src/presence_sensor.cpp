#include <psm/presence_sensor.hpp>

#include <psm/zone.hpp>

namespace psm {

PresenceReading PresenceSensor::read(const std::optional<Item>& item, std::optional<SensorFaultKind> fault) {
    const bool groundTruthOccupied = item.has_value() && item->zone == Zone::PresenceCheck;

    if (!fault.has_value()) {
        if (groundTruthOccupied) {
            lastKnownOccupied_ = true;
        }
        return PresenceReading{ReadingStatus::Ok, groundTruthOccupied};
    }

    if (*fault == SensorFaultKind::Missing) {
        return PresenceReading{ReadingStatus::Missing, false};
    }

    if (lastKnownOccupied_.has_value()) {
        return PresenceReading{ReadingStatus::Stale, *lastKnownOccupied_};
    }
    return PresenceReading{ReadingStatus::Missing, false};
}

}  // namespace psm
