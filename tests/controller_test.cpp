#include "support/check.hpp"
#include <psm/controller.hpp>

int main() {
    psmCheck(psm::classify(100) == psm::WeightClass::Light, "100g classifies as Light");
    psmCheck(psm::classify(499) == psm::WeightClass::Light, "499g classifies as Light");
    psmCheck(psm::classify(500) == psm::WeightClass::Heavy, "500g (at threshold) classifies as Heavy");
    psmCheck(psm::classify(999) == psm::WeightClass::Heavy, "999g classifies as Heavy");

    psmCheck(psm::toDiverterCommand(psm::WeightClass::Light) == psm::DiverterCommand::HoldStraight,
             "Light maps to HoldStraight");
    psmCheck(psm::toDiverterCommand(psm::WeightClass::Heavy) == psm::DiverterCommand::Divert,
             "Heavy maps to Divert");
    return 0;
}
