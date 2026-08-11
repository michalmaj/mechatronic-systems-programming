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
    // TODO (Misja 31: korelacja_per_paczka): jeśli presence.status == ReadingStatus::Ok i
    // presence.occupied, ustaw itemAtPresenceCheck.presenceConfirmed = true. W przeciwnym razie
    // nic nie rób.
    (void)itemAtPresenceCheck;
    (void)presence;
}

void updateClassification(Item& itemAtWeighing, WeightReading weight) {
    // TODO (Misja 31: korelacja_per_paczka): jeśli itemAtWeighing.presenceConfirmed i
    // weight.status == ReadingStatus::Ok, ustaw itemAtWeighing.classification =
    // decideClassification(weight). W przeciwnym razie nic nie rób.
    (void)itemAtWeighing;
    (void)weight;
}

}  // namespace psm
