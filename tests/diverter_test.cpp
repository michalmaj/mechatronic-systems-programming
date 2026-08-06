#include "support/check.hpp"
#include <psm/diverter.hpp>

int main() {
    psm::Diverter diverter;
    psmCheck(diverter.actualPosition() == psm::DiverterPosition::Straight, "starts at Straight");

    diverter.resolve();
    psmCheck(diverter.actualPosition() == psm::DiverterPosition::Straight,
             "resolve is a no-op when already at the commanded target");

    diverter.setCommand(psm::DiverterCommand::Divert);
    psmCheck(diverter.actualPosition() == psm::DiverterPosition::Straight,
             "setCommand alone does not change actual position");

    diverter.resolve();
    psmCheck(diverter.actualPosition() == psm::DiverterPosition::Moving,
             "first resolve after a command change begins moving");

    diverter.resolve();
    psmCheck(diverter.actualPosition() == psm::DiverterPosition::Diverted,
             "second resolve arrives at the commanded target");

    diverter.setCommand(psm::DiverterCommand::HoldStraight);
    diverter.resolve();
    psmCheck(diverter.actualPosition() == psm::DiverterPosition::Moving,
             "changing command again begins a new transition");

    diverter.resolve();
    psmCheck(diverter.actualPosition() == psm::DiverterPosition::Straight,
             "arrives back at Straight");

    return 0;
}
