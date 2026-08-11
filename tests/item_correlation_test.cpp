#include "support/check.hpp"
#include <psm/controller.hpp>

int main() {
    using psm::Item;
    using psm::PresenceReading;
    using psm::ReadingStatus;
    using psm::updateClassification;
    using psm::updatePresenceConfirmation;
    using psm::WeightClass;
    using psm::WeightReading;

    Item atPresenceCheck{1, 750};
    updatePresenceConfirmation(atPresenceCheck, PresenceReading{ReadingStatus::Ok, false});
    psmCheck(!atPresenceCheck.presenceConfirmed, "an Ok-but-not-occupied reading does not confirm presence");

    updatePresenceConfirmation(atPresenceCheck, PresenceReading{ReadingStatus::Stale, true});
    psmCheck(!atPresenceCheck.presenceConfirmed, "a Stale reading never confirms presence, even if occupied=true");

    updatePresenceConfirmation(atPresenceCheck, PresenceReading{ReadingStatus::Ok, true});
    psmCheck(atPresenceCheck.presenceConfirmed, "an Ok, occupied reading confirms presence");

    Item confirmed{2, 750};
    confirmed.presenceConfirmed = true;
    updateClassification(confirmed, WeightReading{ReadingStatus::Ok, 750});
    psmCheck(confirmed.classification.has_value() && *confirmed.classification == WeightClass::Heavy,
             "with presence already confirmed and a trustworthy weight reading, classification is set");

    Item notConfirmed{3, 750};
    updateClassification(notConfirmed, WeightReading{ReadingStatus::Ok, 750});
    psmCheck(!notConfirmed.classification.has_value(),
             "a trustworthy weight reading alone, without presence ever confirmed, does not classify");

    Item confirmedButFaulty{4, 750};
    confirmedButFaulty.presenceConfirmed = true;
    updateClassification(confirmedButFaulty, WeightReading{ReadingStatus::Stale, 750});
    psmCheck(!confirmedButFaulty.classification.has_value(),
             "a degraded weight reading does not classify, even with presence confirmed");

    Item first{5, 100};
    first.presenceConfirmed = true;
    updateClassification(first, WeightReading{ReadingStatus::Ok, 100});
    Item second{6, 800};
    second.presenceConfirmed = true;
    updateClassification(second, WeightReading{ReadingStatus::Ok, 800});
    psmCheck(first.classification.has_value() && *first.classification == WeightClass::Light &&
                 second.classification.has_value() && *second.classification == WeightClass::Heavy,
             "two different concurrently-processed parcels each get their own, independent classification");

    return 0;
}
