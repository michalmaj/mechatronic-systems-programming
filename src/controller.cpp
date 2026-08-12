#include <psm/controller.hpp>

namespace psm {

namespace {
constexpr Grams kHeavyThresholdGrams = 500;
}  // namespace

WeightClass classify(Grams mass) {
    return mass < kHeavyThresholdGrams ? WeightClass::Light : WeightClass::Heavy;
}

DiverterCommand toDiverterCommand(WeightClass weightClass) {
    return weightClass == WeightClass::Light ? DiverterCommand::HoldStraight : DiverterCommand::Divert;
}

std::optional<WeightClass> decideClassification(WeightReading weight) {
    if (weight.status != ReadingStatus::Ok) {
        return std::nullopt;
    }
    return classify(weight.grams);
}

void updatePresenceConfirmation(Item& itemAtPresenceCheck, PresenceReading presence) {
    if (presence.status == ReadingStatus::Ok && presence.occupied) {
        itemAtPresenceCheck.presenceConfirmed = true;
    }
}

void updateClassification(Item& itemAtWeighing, WeightReading weight) {
    if (itemAtWeighing.presenceConfirmed && weight.status == ReadingStatus::Ok) {
        itemAtWeighing.classification = decideClassification(weight);
    }
}

}  // namespace psm
