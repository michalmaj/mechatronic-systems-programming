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
    // TODO (Misja 22: klasyfikacja_odporna_na_awarie): zwróć nullopt, chyba że
    // weight.status == ReadingStatus::Ok; w przeciwnym razie classify(weight.grams).
    (void)weight;
    return std::nullopt;
}

}  // namespace psm
