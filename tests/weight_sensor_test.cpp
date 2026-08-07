#include "support/check.hpp"
#include <psm/weight_sensor.hpp>

int main() {
    using psm::FaultKind;
    using psm::Item;
    using psm::ReadingStatus;
    using psm::WeightSensor;
    using psm::Zone;

    WeightSensor sensor;

    auto reading = sensor.read(std::nullopt, std::nullopt);
    psmCheck(reading.status == ReadingStatus::Ok, "no fault, no item: status is Ok");
    psmCheck(reading.grams == 0, "no fault, no item: reads zero");

    Item item{1, Zone::Weighing, 750};
    reading = sensor.read(item, std::nullopt);
    psmCheck(reading.status == ReadingStatus::Ok, "no fault, item present: status is Ok");
    psmCheck(reading.grams == 750, "no fault, item present: reads the true mass");

    reading = sensor.read(item, FaultKind::Missing);
    psmCheck(reading.status == ReadingStatus::Missing, "Missing fault reports Missing status");

    reading = sensor.read(std::nullopt, FaultKind::Stale);
    psmCheck(reading.status == ReadingStatus::Stale, "Stale fault reports Stale status");
    psmCheck(reading.grams == 750, "Stale fault repeats the last known-good reading");

    reading = sensor.read(item, FaultKind::OutOfRange);
    psmCheck(reading.status == ReadingStatus::OutOfRange, "OutOfRange fault reports OutOfRange status");
    psmCheck(reading.grams < 0, "OutOfRange fault reports a physically implausible value");

    reading = sensor.read(item, FaultKind::Noisy);
    psmCheck(reading.status == ReadingStatus::Ok, "Noisy fault keeps status Ok -- only the value is corrupted");
    psmCheck(reading.grams != item.mass, "Noisy fault perturbs the reported value away from the true mass");

    return 0;
}
