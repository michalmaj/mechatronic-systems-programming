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

    psmCheck(psm::toString(psm::ReadingStatus::Ok) == "Ok", "Ok prints as Ok");
    psmCheck(psm::toString(psm::ReadingStatus::Missing) == "Missing", "Missing prints as Missing");
    psmCheck(psm::toString(psm::ReadingStatus::Stale) == "Stale", "Stale prints as Stale");
    psmCheck(psm::toString(psm::ReadingStatus::OutOfRange) == "OutOfRange", "OutOfRange prints as OutOfRange");

    return 0;
}
