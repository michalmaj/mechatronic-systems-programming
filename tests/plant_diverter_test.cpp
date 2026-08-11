#include "support/check.hpp"
#include <psm/diverter.hpp>
#include <psm/plant.hpp>

int main() {
    // -- Late-command scenario: the diverter is only told to divert once the parcel is already
    // waiting at diverting -- the only way to actually observe Plant waiting on the diverter.
    psm::Plant plant;
    psm::Diverter diverter;
    psm::spawnItem(plant, 1, 750);

    diverter.resolve();
    psm::advance(plant, diverter);
    psmCheck(plant.presenceCheck.has_value() && plant.presenceCheck->id == 1, "reaches presenceCheck");

    diverter.resolve();
    psm::advance(plant, diverter);
    psmCheck(plant.weighing.has_value() && plant.weighing->id == 1, "reaches weighing");

    diverter.resolve();
    psm::advance(plant, diverter);
    psmCheck(plant.diverting.has_value() && plant.diverting->id == 1, "reaches diverting, still on HoldStraight");
    psmCheck(diverter.isSettled(), "diverter was already settled at Straight the whole time");

    diverter.setCommand(psm::DiverterCommand::Divert);
    psmCheck(!diverter.isSettled(), "unsettled the moment the command changes");

    diverter.resolve();
    auto waiting = psm::advance(plant, diverter);
    psmCheck(!waiting.departure.has_value(), "no departure while the diverter is still Moving");
    psmCheck(plant.diverting.has_value() && plant.diverting->id == 1,
             "parcel waits at diverting while the diverter is still Moving");

    diverter.resolve();
    auto routed = psm::advance(plant, diverter);
    psmCheck(routed.departure.has_value() && routed.departure->id == 1 &&
                 routed.departure->destination == psm::Zone::OutputHeavy,
             "parcel routes only once the diverter has actually settled");
    psmCheck(!plant.diverting.has_value(), "diverting is empty immediately after departure -- Output is a sink");

    return 0;
}
