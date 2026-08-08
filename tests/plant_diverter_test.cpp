#include "support/check.hpp"
#include <psm/controller.hpp>
#include <psm/diverter.hpp>
#include <psm/loop.hpp>
#include <psm/plant.hpp>

int main() {
    // -- Eager scenario: the controller decides from tick 0, same as runTicks always has. The
    // diverter settles (2 ticks) well within the 3 ticks the parcel needs to reach Diverting, so
    // this looks identical to Module 1's timing.
    {
        psm::Plant plant;
        psm::spawnItem(plant, psm::Item{1, psm::Zone::Infeed, 750});

        psm::runTicks(plant, 4);
        psmCheck(plant.item.has_value() && plant.item->zone == psm::Zone::OutputHeavy,
                 "eager decision: a 750g parcel reaches OutputHeavy after 4 ticks, same as Module 1");

        psm::runTicks(plant, 1);
        psmCheck(!plant.item.has_value(), "one more tick clears the item from the system");
    }

    // -- Late-command scenario: the diverter is only told to divert once the parcel is already
    // waiting at Diverting -- the only way to actually observe Plant waiting on the diverter.
    {
        psm::Plant plant;
        psm::Diverter diverter;
        psm::spawnItem(plant, psm::Item{2, psm::Zone::Infeed, 750});

        diverter.resolve();
        psm::advance(plant, diverter);
        psmCheck(plant.item->zone == psm::Zone::PresenceCheck, "reaches PresenceCheck");

        diverter.resolve();
        psm::advance(plant, diverter);
        psmCheck(plant.item->zone == psm::Zone::Weighing, "reaches Weighing");

        diverter.resolve();
        psm::advance(plant, diverter);
        psmCheck(plant.item->zone == psm::Zone::Diverting, "reaches Diverting, still on HoldStraight");
        psmCheck(diverter.isSettled(), "diverter was already settled at Straight the whole time");

        diverter.setCommand(psm::DiverterCommand::Divert);
        psmCheck(!diverter.isSettled(), "unsettled the moment the command changes");

        diverter.resolve();
        psm::advance(plant, diverter);
        psmCheck(plant.item->zone == psm::Zone::Diverting,
                 "parcel waits at Diverting while the diverter is still Moving");

        diverter.resolve();
        psm::advance(plant, diverter);
        psmCheck(plant.item->zone == psm::Zone::OutputHeavy,
                 "parcel routes only once the diverter has actually settled");
    }

    return 0;
}
