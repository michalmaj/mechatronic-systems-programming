#include <psm/presence_sensor.hpp>

namespace psm {

PresenceReading PresenceSensor::read(const std::optional<Item>& item, std::optional<SensorFaultKind> fault) {
    // TODO (Misja 31: korelacja_per_paczka): ground truth to teraz po prostu item.has_value() --
    // ten sensor czyta dokładnie ten slot Plant, do którego jest fizycznie przypięty, więc nie ma
    // już potrzeby porównywania item->zone (Item nie ma już zone). Zachowaj resztę logiki Modułu 6
    // bez zmian: brak usterki -> Ok i (jeśli occupied) zapamiętaj w lastKnownOccupied_; fault ==
    // Missing -> {Missing, false}; w przeciwnym razie (Stale) zwróć lastKnownOccupied_, jeśli
    // istnieje, inaczej zdegraduj do {Missing, false}.
    (void)item;
    (void)fault;
    return PresenceReading{ReadingStatus::Missing, false};
}

}  // namespace psm
