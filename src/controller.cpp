#include <psm/controller.hpp>

namespace psm {

namespace {
constexpr Grams kHeavyThresholdGrams = 500;
}  // namespace

WeightClass classify(Grams mass) {
    return mass < kHeavyThresholdGrams ? WeightClass::Light : WeightClass::Heavy;
}

DiverterPosition toDiverterPosition(WeightClass weightClass) {
    return weightClass == WeightClass::Light ? DiverterPosition::Straight : DiverterPosition::Diverted;
}

}  // namespace psm
