#include "support/check.hpp"
#include <psm/controller.hpp>

int main() {
    psmCheck(psm::classify(100) == psm::WeightClass::Light, "100g classifies as Light");
    psmCheck(psm::classify(499) == psm::WeightClass::Light, "499g classifies as Light");
    psmCheck(psm::classify(500) == psm::WeightClass::Heavy, "500g (at threshold) classifies as Heavy");
    psmCheck(psm::classify(999) == psm::WeightClass::Heavy, "999g classifies as Heavy");

    psmCheck(psm::toDiverterCommand(psm::WeightClass::Light) == psm::DiverterCommand::HoldStraight,
             "Light maps to HoldStraight");
    psmCheck(psm::toDiverterCommand(psm::WeightClass::Heavy) == psm::DiverterCommand::Divert,
             "Heavy maps to Divert");

    using psm::PresenceReading;
    using psm::ReadingStatus;
    using psm::WeightReading;

    auto result = psm::decideClassification(PresenceReading{ReadingStatus::Ok, true},
                                              WeightReading{ReadingStatus::Ok, 750});
    psmCheck(result.has_value() && *result == psm::WeightClass::Heavy,
             "classifies normally when both readings are Ok and something is present");

    result = psm::decideClassification(PresenceReading{ReadingStatus::Ok, false},
                                        WeightReading{ReadingStatus::Ok, 750});
    psmCheck(!result.has_value(), "does not classify when presence says nothing is occupying the sensor");

    result = psm::decideClassification(PresenceReading{ReadingStatus::Missing, false},
                                        WeightReading{ReadingStatus::Ok, 750});
    psmCheck(!result.has_value(), "does not classify when the presence reading itself is degraded");

    result = psm::decideClassification(PresenceReading{ReadingStatus::Ok, true},
                                        WeightReading{ReadingStatus::Stale, 750});
    psmCheck(!result.has_value(), "does not classify when the weight reading is degraded, even if presence is fine");

    return 0;
}
