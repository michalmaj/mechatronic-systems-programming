#include "support/check.hpp"
#include <psm/presence_sensor.hpp>

int main() {
    using psm::FaultKind;
    using psm::Item;
    using psm::PresenceSensor;
    using psm::ReadingStatus;
    using psm::Zone;

    PresenceSensor sensor;

    auto reading = sensor.read(std::nullopt, std::nullopt);
    psmCheck(reading.status == ReadingStatus::Ok, "no fault, no item: status is Ok");
    psmCheck(!reading.occupied, "no fault, no item: not occupied");

    Item item{1, Zone::PresenceCheck, 750};
    reading = sensor.read(item, std::nullopt);
    psmCheck(reading.status == ReadingStatus::Ok, "no fault, item present: status is Ok");
    psmCheck(reading.occupied, "no fault, item present: occupied");

    reading = sensor.read(item, FaultKind::Missing);
    psmCheck(reading.status == ReadingStatus::Missing, "Missing fault reports Missing status");
    psmCheck(!reading.occupied, "Missing fault reports not occupied");

    reading = sensor.read(std::nullopt, FaultKind::Stale);
    psmCheck(reading.status == ReadingStatus::Stale, "Stale fault reports Stale status");
    psmCheck(reading.occupied,
             "Stale fault repeats the last known-good reading (occupied, from before the Missing fault)");

    return 0;
}
