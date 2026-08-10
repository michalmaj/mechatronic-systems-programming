#include "support/check.hpp"
#include <psm/controller_state.hpp>

int main() {
    using psm::ControllerState;
    using psm::Item;
    using psm::PresenceReading;
    using psm::ReadingStatus;
    using psm::updateControllerState;
    using psm::WeightClass;
    using psm::WeightReading;
    using psm::Zone;

    ControllerState state;

    Item item{1, Zone::Infeed, 750};
    updateControllerState(state, item, PresenceReading{ReadingStatus::Ok, false}, WeightReading{ReadingStatus::Ok, 0});
    psmCheck(!state.presenceConfirmed, "at Infeed: presence not yet confirmed");
    psmCheck(!state.classification.has_value(), "at Infeed: no classification yet");

    item.zone = Zone::PresenceCheck;
    updateControllerState(state, item, PresenceReading{ReadingStatus::Ok, true}, WeightReading{ReadingStatus::Ok, 0});
    psmCheck(state.presenceConfirmed, "at PresenceCheck with a trustworthy reading: presence becomes confirmed");

    item.zone = Zone::Weighing;
    updateControllerState(state, item, PresenceReading{ReadingStatus::Ok, false}, WeightReading{ReadingStatus::Ok, 750});
    psmCheck(state.classification.has_value() && *state.classification == WeightClass::Heavy,
             "at Weighing with presence already confirmed and a trustworthy weight reading: classification is set");

    ControllerState midway;
    midway.presenceConfirmed = true;
    Item freshItem{2, Zone::Infeed, 100};
    updateControllerState(midway, freshItem, PresenceReading{ReadingStatus::Ok, true}, WeightReading{ReadingStatus::Ok, 100});
    psmCheck(!midway.presenceConfirmed, "a new parcel at Infeed resets presenceConfirmed from any prior state");
    psmCheck(!midway.classification.has_value(), "a new parcel at Infeed resets classification from any prior state");

    ControllerState neverConfirmed;
    Item item2{3, Zone::Weighing, 750};
    updateControllerState(neverConfirmed, item2, PresenceReading{ReadingStatus::Ok, false}, WeightReading{ReadingStatus::Ok, 750});
    psmCheck(!neverConfirmed.classification.has_value(),
             "a trustworthy weight reading alone, without presence ever confirmed, does not produce a classification");

    ControllerState confirmedButFaulty;
    confirmedButFaulty.presenceConfirmed = true;
    Item item3{4, Zone::Weighing, 750};
    updateControllerState(confirmedButFaulty, item3, PresenceReading{ReadingStatus::Ok, false},
                           WeightReading{ReadingStatus::Stale, 750});
    psmCheck(!confirmedButFaulty.classification.has_value(),
             "a degraded weight reading at Weighing does not produce a classification, even with presence confirmed");

    return 0;
}
