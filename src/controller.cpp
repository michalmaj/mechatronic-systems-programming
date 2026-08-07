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

std::optional<WeightClass> decideClassification(PresenceReading presence, WeightReading weight) {
    if (presence.status != ReadingStatus::Ok || !presence.occupied) {
        return std::nullopt;
    }
    if (weight.status != ReadingStatus::Ok) {
        return std::nullopt;
    }
    return classify(weight.grams);
}

}  // namespace psm
