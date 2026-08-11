#include <psm/weight_sensor.hpp>

namespace psm {

WeightReading WeightSensor::read(const std::optional<Item>& item, std::optional<SensorFaultKind> fault) {
    // TODO (Misja 31: korelacja_per_paczka): ground truth to teraz po prostu item.has_value()
    // (i item->mass, jeśli obecny) -- ten sensor czyta dokładnie ten slot Plant, do którego jest
    // fizycznie przypięty. Zachowaj resztę logiki Modułu 6 bez zmian: brak usterki -> Ok i (jeśli
    // obecny) zapamiętaj w lastKnownMass_; fault == Missing -> {Missing, 0}; w przeciwnym razie
    // (Stale) zwróć lastKnownMass_, jeśli istnieje, inaczej zdegraduj do {Missing, 0}.
    (void)item;
    (void)fault;
    return WeightReading{ReadingStatus::Missing, 0};
}

}  // namespace psm
