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

    // Blocked fault: resolve() is a complete no-op while active, even from an already-settled state.
    psm::Diverter blockedDiverter;
    psmCheck(blockedDiverter.isSettled(), "starts settled (HoldStraight command, Straight actual)");

    blockedDiverter.setCommand(psm::DiverterCommand::Divert);
    psmCheck(!blockedDiverter.isSettled(), "no longer settled once commanded away from the actual position");

    blockedDiverter.resolve(psm::FaultKind::Blocked);
    psmCheck(blockedDiverter.actualPosition() == psm::DiverterPosition::Straight,
             "Blocked fault prevents any movement, even the first step toward Moving");
    psmCheck(!blockedDiverter.isSettled(), "still not settled while blocked");

    blockedDiverter.resolve(psm::FaultKind::Blocked);
    psmCheck(blockedDiverter.actualPosition() == psm::DiverterPosition::Straight,
             "stays blocked across multiple consecutive ticks");

    blockedDiverter.resolve();
    psmCheck(blockedDiverter.actualPosition() == psm::DiverterPosition::Moving,
             "once unblocked, resolve proceeds normally from wherever it was");

    blockedDiverter.resolve();
    psmCheck(blockedDiverter.isSettled(), "eventually settles once unblocked");

    return 0;
}
