#include <psm/controller.hpp>

namespace psm {

namespace {
constexpr Grams kHeavyThresholdGrams = 500;
}  // namespace

WeightClass classify(Grams mass) {
    return mass < kHeavyThresholdGrams ? WeightClass::Light : WeightClass::Heavy;
}

DiverterCommand toDiverterCommand(WeightClass weightClass) {
    // TODO (Misja 7: polecenie_a_rzeczywistosc): zmapuj Light -> HoldStraight, Heavy -> Divert.
    (void)weightClass;
    return DiverterCommand::HoldStraight;
}

}  // namespace psm
