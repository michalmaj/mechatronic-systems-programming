#include "support/check.hpp"
#include <psm/weight_sensor.hpp>

int main() {
    using psm::Item;
    using psm::ReadingStatus;
    using psm::SensorFaultKind;
    using psm::WeightSensor;

    WeightSensor sensor;

    auto reading = sensor.read(std::nullopt, std::nullopt);
    psmCheck(reading.status == ReadingStatus::Ok, "no item, no fault: status is Ok");
    psmCheck(reading.grams == 0, "no item: reads zero");

    Item present{1, 750};
    reading = sensor.read(present, std::nullopt);
    psmCheck(reading.status == ReadingStatus::Ok, "item present, no fault: status is Ok");
    psmCheck(reading.grams == 750, "item present: reads the true mass, and this becomes the trusted reading");

    reading = sensor.read(std::nullopt, std::nullopt);
    psmCheck(reading.status == ReadingStatus::Ok, "slot empty again: status is still Ok");
    psmCheck(reading.grams == 0, "slot empty: nothing on the scale, reads zero");

    reading = sensor.read(std::nullopt, SensorFaultKind::Stale);
    psmCheck(reading.status == ReadingStatus::Stale, "Stale fault reports Stale status");
    psmCheck(reading.grams == 750,
             "Stale still repeats the earlier trusted reading -- the confirmed-empty read never touched memory");

    reading = sensor.read(present, SensorFaultKind::Missing);
    psmCheck(reading.status == ReadingStatus::Missing, "Missing fault reports Missing status");

    WeightSensor freshSensor;
    reading = freshSensor.read(present, SensorFaultKind::Stale);
    psmCheck(reading.status == ReadingStatus::Missing, "Stale with no prior trusted reading degrades to Missing");

    return 0;
}
