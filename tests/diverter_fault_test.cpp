#include "support/check.hpp"
#include <psm/diverter.hpp>

int main() {
    using psm::Diverter;
    using psm::DiverterCommand;
    using psm::DiverterFaultKind;
    using psm::DiverterPosition;

    {
        Diverter diverter;
        diverter.setCommand(DiverterCommand::Divert);
        psmCheck(!diverter.isSettled(), "unsettled the moment the command changes");

        diverter.resolve(DiverterFaultKind::Blocked);
        psmCheck(diverter.actualPosition() == DiverterPosition::Straight,
                 "Blocked: actual position does not change at all, not even toward Moving");
        psmCheck(!diverter.isSettled(), "still not settled -- it never even started moving");

        diverter.resolve(DiverterFaultKind::Blocked);
        psmCheck(diverter.actualPosition() == DiverterPosition::Straight,
                 "still Blocked on a second resolve(): still no change");
    }
    {
        Diverter diverter;
        diverter.setCommand(DiverterCommand::Divert);
        diverter.resolve();
        psmCheck(diverter.actualPosition() == DiverterPosition::Moving, "first unfaulted resolve() begins moving");

        diverter.resolve(DiverterFaultKind::Blocked);
        psmCheck(diverter.actualPosition() == DiverterPosition::Moving,
                 "a fault injected mid-transition freezes the diverter exactly where it was, still Moving");
        psmCheck(!diverter.isSettled(), "Moving is never settled, blocked or not");

        diverter.resolve();
        psmCheck(diverter.actualPosition() == DiverterPosition::Diverted,
                 "once the fault is cleared (nullopt again), the diverter finishes the transition it was "
                 "already in");
        psmCheck(diverter.isSettled(), "settled once actual position matches the command's target");
    }
    {
        Diverter diverter;
        diverter.setCommand(DiverterCommand::Divert);
        diverter.resolve();
        diverter.resolve();
        psmCheck(diverter.isSettled(), "settled without any fault, exactly as in Module 2");
    }
    return 0;
}
