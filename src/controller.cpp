#include <psm/controller.hpp>

namespace psm {

WeightClass classify(Grams mass) {
    // TODO (Misja 4: decyzja_sortowania): porównaj mass z progiem (500g) i zwróć
    // WeightClass::Light albo WeightClass::Heavy.
    (void)mass;
    return WeightClass::Light;
}

DiverterPosition toDiverterPosition(WeightClass weightClass) {
    // TODO (Misja 4: decyzja_sortowania): zmapuj Light -> Straight, Heavy -> Diverted.
    (void)weightClass;
    return DiverterPosition::Straight;
}

}  // namespace psm
