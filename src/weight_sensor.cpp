#include <psm/weight_sensor.hpp>

#include <psm/zone.hpp>

namespace psm {

WeightReading WeightSensor::read(const std::optional<Item>& item, std::optional<SensorFaultKind> fault) {
    const bool onScale = item.has_value() && item->zone == Zone::Weighing;
    const Grams groundTruthMass = onScale ? item->mass : 0;

    if (!fault.has_value()) {
        if (onScale) {
            lastKnownMass_ = groundTruthMass;
        }
        return WeightReading{ReadingStatus::Ok, groundTruthMass};
    }

    if (*fault == SensorFaultKind::Missing) {
        return WeightReading{ReadingStatus::Missing, 0};
    }

    if (lastKnownMass_.has_value()) {
        return WeightReading{ReadingStatus::Stale, *lastKnownMass_};
    }
    return WeightReading{ReadingStatus::Missing, 0};
}

}  // namespace psm
