#include "support/check.hpp"
#include <psm/presence_sensor.hpp>

int main() {
    using psm::Item;
    using psm::PresenceSensor;
    using psm::ReadingStatus;
    using psm::SensorFaultKind;

    PresenceSensor sensor;

    auto reading = sensor.read(std::nullopt, std::nullopt);
    psmCheck(reading.status == ReadingStatus::Ok, "no item, no fault: status is Ok");
    psmCheck(!reading.occupied, "no item: not occupied");

    Item present{1, 750};
    reading = sensor.read(present, std::nullopt);
    psmCheck(reading.status == ReadingStatus::Ok, "item present, no fault: status is Ok");
    psmCheck(reading.occupied, "item present: occupied, and this becomes the trusted reading");

    reading = sensor.read(std::nullopt, std::nullopt);
    psmCheck(reading.status == ReadingStatus::Ok, "slot empty again: status is still Ok");
    psmCheck(!reading.occupied, "slot empty: ground truth here is not occupied");

    reading = sensor.read(std::nullopt, SensorFaultKind::Stale);
    psmCheck(reading.status == ReadingStatus::Stale, "Stale fault reports Stale status");
    psmCheck(reading.occupied,
             "Stale still repeats the earlier trusted reading -- the confirmed-empty read never touched memory");

    reading = sensor.read(present, SensorFaultKind::Missing);
    psmCheck(reading.status == ReadingStatus::Missing, "Missing fault reports Missing status");
    psmCheck(!reading.occupied, "Missing fault reports not occupied");

    PresenceSensor freshSensor;
    reading = freshSensor.read(present, SensorFaultKind::Stale);
    psmCheck(reading.status == ReadingStatus::Missing, "Stale with no prior trusted reading degrades to Missing");
    psmCheck(!reading.occupied, "degraded Missing reading reports not occupied");

    return 0;
}
