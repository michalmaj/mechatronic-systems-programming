#include "support/check.hpp"
#include <psm/presence_sensor.hpp>

int main() {
    using psm::SensorFaultKind;
    using psm::Item;
    using psm::PresenceSensor;
    using psm::ReadingStatus;
    using psm::Zone;

    PresenceSensor sensor;

    auto reading = sensor.read(std::nullopt, std::nullopt);
    psmCheck(reading.status == ReadingStatus::Ok, "no item, no fault: status is Ok");
    psmCheck(!reading.occupied, "no item: not occupied");

    Item atPresenceCheck{1, Zone::PresenceCheck, 750};
    reading = sensor.read(atPresenceCheck, std::nullopt);
    psmCheck(reading.status == ReadingStatus::Ok, "item at PresenceCheck, no fault: status is Ok");
    psmCheck(reading.occupied, "item at PresenceCheck: occupied, and this becomes the trusted reading");

    Item atWeighing{1, Zone::Weighing, 750};
    reading = sensor.read(atWeighing, std::nullopt);
    psmCheck(reading.status == ReadingStatus::Ok, "item elsewhere: status is still Ok");
    psmCheck(!reading.occupied, "item at Weighing (not PresenceCheck): ground truth here is not occupied");

    reading = sensor.read(atWeighing, SensorFaultKind::Stale);
    psmCheck(reading.status == ReadingStatus::Stale, "Stale fault reports Stale status");
    psmCheck(reading.occupied,
             "Stale still repeats the PresenceCheck reading -- the 'elsewhere' read never touched memory");

    reading = sensor.read(atPresenceCheck, SensorFaultKind::Missing);
    psmCheck(reading.status == ReadingStatus::Missing, "Missing fault reports Missing status");
    psmCheck(!reading.occupied, "Missing fault reports not occupied");

    PresenceSensor freshSensor;
    reading = freshSensor.read(atPresenceCheck, SensorFaultKind::Stale);
    psmCheck(reading.status == ReadingStatus::Missing,
             "Stale with no prior trusted reading degrades to Missing");
    psmCheck(!reading.occupied, "degraded Missing reading reports not occupied");

    return 0;
}
