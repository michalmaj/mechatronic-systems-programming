#include <psm/presence_sensor.hpp>

namespace psm {

PresenceReading PresenceSensor::read(const std::optional<Item>& item, std::optional<FaultKind> fault) {
    // TODO (Misja 20: czujnik_obecnosci): ground truth to item.has_value() && item->zone ==
    // Zone::PresenceCheck. Bez usterki: zapisz do lastKnownOccupied_ i zwróć {Ok, ground truth}.
    // Missing: zwróć {Missing, false}, nie dotykaj pamięci. Stale: jeśli lastKnownOccupied_ ma
    // wartość, zwróć {Stale, *lastKnownOccupied_}; w przeciwnym razie zwróć {Missing, false}
    // (nigdy nie było czego powtórzyć).
    (void)item;
    (void)fault;
    return PresenceReading{ReadingStatus::Missing, false};
}

}  // namespace psm
