#include "support/check.hpp"
#include <psm/weight_sensor.hpp>

int main() {
    using psm::SensorFaultKind;
    using psm::Item;
    using psm::ReadingStatus;
    using psm::WeightSensor;
    using psm::Zone;

    WeightSensor sensor;

    auto reading = sensor.read(std::nullopt, std::nullopt);
    psmCheck(reading.status == ReadingStatus::Ok, "no item, no fault: status is Ok");
    psmCheck(reading.grams == 0, "no item: reads zero");

    Item atWeighing{1, Zone::Weighing, 750};
    reading = sensor.read(atWeighing, std::nullopt);
    psmCheck(reading.status == ReadingStatus::Ok, "item at Weighing, no fault: status is Ok");
    psmCheck(reading.grams == 750, "item at Weighing: reads the true mass, and this becomes the trusted reading");

    Item atDiverting{1, Zone::Diverting, 750};
    reading = sensor.read(atDiverting, std::nullopt);
    psmCheck(reading.status == ReadingStatus::Ok, "item elsewhere: status is still Ok");
    psmCheck(reading.grams == 0, "item at Diverting (not Weighing): nothing on the scale, reads zero");

    reading = sensor.read(atDiverting, SensorFaultKind::Stale);
    psmCheck(reading.status == ReadingStatus::Stale, "Stale fault reports Stale status");
    psmCheck(reading.grams == 750,
             "Stale still repeats the Weighing reading -- the 'elsewhere' read never touched memory");

    reading = sensor.read(atWeighing, SensorFaultKind::Missing);
    psmCheck(reading.status == ReadingStatus::Missing, "Missing fault reports Missing status");

    WeightSensor freshSensor;
    reading = freshSensor.read(atWeighing, SensorFaultKind::Stale);
    psmCheck(reading.status == ReadingStatus::Missing,
             "Stale with no prior trusted reading degrades to Missing");

    return 0;
}
