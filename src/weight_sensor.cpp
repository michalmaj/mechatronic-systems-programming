#include <psm/weight_sensor.hpp>

namespace psm {

WeightReading WeightSensor::read(const std::optional<Item>& item, std::optional<FaultKind> fault) {
    // TODO (Misja 21: czujnik_wagi): ten sam wzorzec co PresenceSensor (Misja 20), ale ground
    // truth to item.has_value() && item->zone == Zone::Weighing -> item->mass, a pamięć to
    // lastKnownMass_ (Grams). Bez usterki, nie na wadze: zwróć {Ok, 0}, nie dotykaj pamięci.
    (void)item;
    (void)fault;
    return WeightReading{ReadingStatus::Missing, 0};
}

}  // namespace psm
