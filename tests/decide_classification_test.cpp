#include "support/check.hpp"
#include <psm/controller.hpp>

int main() {
    using psm::decideClassification;
    using psm::ReadingStatus;
    using psm::WeightClass;
    using psm::WeightReading;

    auto result = decideClassification(WeightReading{ReadingStatus::Ok, 750});
    psmCheck(result.has_value() && *result == WeightClass::Heavy, "Ok reading, heavy mass: classifies as Heavy");

    result = decideClassification(WeightReading{ReadingStatus::Ok, 100});
    psmCheck(result.has_value() && *result == WeightClass::Light, "Ok reading, light mass: classifies as Light");

    result = decideClassification(WeightReading{ReadingStatus::Missing, 750});
    psmCheck(!result.has_value(), "Missing reading: no classification, regardless of the number");

    result = decideClassification(WeightReading{ReadingStatus::Stale, 750});
    psmCheck(!result.has_value(), "Stale reading: no classification, even though the number looks plausible");

    return 0;
}
