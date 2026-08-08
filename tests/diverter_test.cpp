#include "support/check.hpp"
#include <psm/diverter.hpp>

int main() {
    {
        psm::Diverter diverter;
        psmCheck(diverter.actualPosition() == psm::DiverterPosition::Straight, "starts at Straight");
        psmCheck(diverter.isSettled(), "starts settled: default command is HoldStraight");
    }
    {
        psm::Diverter diverter;
        diverter.setCommand(psm::DiverterCommand::Divert);
        psmCheck(!diverter.isSettled(), "unsettled the moment the command changes, before resolve()");

        diverter.resolve();
        psmCheck(diverter.actualPosition() == psm::DiverterPosition::Moving, "first resolve() begins moving");
        psmCheck(!diverter.isSettled(), "Moving is never settled");

        diverter.resolve();
        psmCheck(diverter.actualPosition() == psm::DiverterPosition::Diverted, "second resolve() arrives");
        psmCheck(diverter.isSettled(), "settled once actual position matches the command's target");
    }
    {
        psm::Diverter diverter;
        diverter.setCommand(psm::DiverterCommand::Divert);
        diverter.resolve();
        diverter.resolve();
        psmCheck(diverter.actualPosition() == psm::DiverterPosition::Diverted, "settled at Diverted");

        diverter.setCommand(psm::DiverterCommand::HoldStraight);
        psmCheck(!diverter.isSettled(), "unsettled again immediately after the command flips back");

        diverter.resolve();
        psmCheck(diverter.actualPosition() == psm::DiverterPosition::Moving, "moves again toward Straight");

        diverter.resolve();
        psmCheck(diverter.actualPosition() == psm::DiverterPosition::Straight, "arrives back at Straight");
        psmCheck(diverter.isSettled(), "settled again");
    }
    return 0;
}
