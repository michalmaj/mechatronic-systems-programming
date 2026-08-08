#include "support/check.hpp"
#include <psm/controller.hpp>

int main() {
    psmCheck(psm::toDiverterCommand(psm::WeightClass::Light) == psm::DiverterCommand::HoldStraight,
             "Light maps to HoldStraight");
    psmCheck(psm::toDiverterCommand(psm::WeightClass::Heavy) == psm::DiverterCommand::Divert,
             "Heavy maps to Divert");
    return 0;
}
