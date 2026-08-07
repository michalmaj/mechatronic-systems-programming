#include "support/check.hpp"
#include <psm/sensor_snapshot.hpp>

int main() {
    psm::PresenceReading presence{psm::ReadingStatus::Ok, true};
    psmCheck(presence.status == psm::ReadingStatus::Ok, "PresenceReading stores status");
    psmCheck(presence.occupied, "PresenceReading stores occupied");

    psm::WeightReading weight{psm::ReadingStatus::Stale, 500};
    psmCheck(weight.status == psm::ReadingStatus::Stale, "WeightReading stores status");
    psmCheck(weight.grams == 500, "WeightReading stores grams");

    psm::SensorSnapshot snapshot{7, presence, weight};
    psmCheck(snapshot.observedAt == 7, "SensorSnapshot stores observedAt");
    psmCheck(snapshot.presence.occupied, "SensorSnapshot stores the presence reading");
    psmCheck(snapshot.weight.grams == 500, "SensorSnapshot stores the weight reading");

    return 0;
}
