#include <psm/weight_sensor.hpp>

namespace psm {

namespace {
constexpr Grams kOutOfRangeGrams = -1;
constexpr Grams kNoisyOffsetGrams = 50;
}  // namespace

WeightReading WeightSensor::read(const std::optional<Item>& item, std::optional<FaultKind> fault) {
    const Grams groundTruth = item.has_value() ? item->mass : 0;

    if (!fault.has_value()) {
        lastKnownGrams_ = groundTruth;
        return WeightReading{ReadingStatus::Ok, groundTruth};
    }

    switch (*fault) {
        case FaultKind::Missing:
            return WeightReading{ReadingStatus::Missing, 0};
        case FaultKind::Stale:
            return WeightReading{ReadingStatus::Stale, lastKnownGrams_};
        case FaultKind::OutOfRange:
            return WeightReading{ReadingStatus::OutOfRange, kOutOfRangeGrams};
        case FaultKind::Noisy:
            return WeightReading{ReadingStatus::Ok, static_cast<Grams>(groundTruth + kNoisyOffsetGrams)};
        case FaultKind::Blocked:
            break;  // not a valid WeightSensor fault per isValidFaultScript -- unreachable in practice
    }
    return WeightReading{ReadingStatus::Ok, groundTruth};
}

}  // namespace psm
