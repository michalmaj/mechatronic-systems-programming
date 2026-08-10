#include "support/check.hpp"
#include <psm/diverter.hpp>
#include <psm/plant.hpp>
#include <psm/system_event_kind.hpp>

int main() {
    using psm::Diverter;
    using psm::DiverterCommand;
    using psm::DiverterFaultKind;
    using psm::Item;
    using psm::Plant;
    using psm::SystemEventKind;
    using psm::Zone;

    // -- A permanently blocked diverter: first attempt DiverterNotReady, second (consecutive)
    // attempt RoutingDeadlineMissed -- the frozen deadline timing.
    {
        Plant plant;
        Diverter diverter;
        psm::spawnItem(plant, Item{1, Zone::Infeed, 750});
        diverter.setCommand(DiverterCommand::Divert);

        for (int i = 0; i < 3; ++i) {
            diverter.resolve(DiverterFaultKind::Blocked);
            psm::advance(plant, diverter, true);
        }
        psmCheck(plant.item->zone == Zone::Diverting, "item reaches Diverting");
        psmCheck(plant.divertingWaitTicks == 0, "counter starts at 0 the moment the item enters Diverting");

        diverter.resolve(DiverterFaultKind::Blocked);
        auto first = psm::advance(plant, diverter, true);
        psmCheck(first.has_value() && *first == SystemEventKind::DiverterNotReady,
                 "first unsettled active attempt: DiverterNotReady");
        psmCheck(plant.item->zone == Zone::Diverting, "item still waits");

        diverter.resolve(DiverterFaultKind::Blocked);
        auto second = psm::advance(plant, diverter, true);
        psmCheck(second.has_value() && *second == SystemEventKind::RoutingDeadlineMissed,
                 "second consecutive unsettled active attempt: RoutingDeadlineMissed");
        psmCheck(plant.item->zone == Zone::Diverting, "item still waits -- advance() never routes on its own");
    }

    // -- !routingReady resets the counter instead of freezing it: an interrupted attempt gets a
    // full, fresh grace period once routing resumes.
    {
        Plant plant;
        Diverter diverter;
        psm::spawnItem(plant, Item{1, Zone::Infeed, 750});
        diverter.setCommand(DiverterCommand::Divert);

        for (int i = 0; i < 3; ++i) {
            diverter.resolve(DiverterFaultKind::Blocked);
            psm::advance(plant, diverter, true);
        }
        psmCheck(plant.item->zone == Zone::Diverting, "item reaches Diverting");

        diverter.resolve(DiverterFaultKind::Blocked);
        auto notReady = psm::advance(plant, diverter, true);
        psmCheck(notReady.has_value() && *notReady == SystemEventKind::DiverterNotReady,
                 "one active unsettled attempt: DiverterNotReady, counter now at 1");

        diverter.resolve(DiverterFaultKind::Blocked);
        auto paused = psm::advance(plant, diverter, false);
        psmCheck(!paused.has_value(), "a paused (routingReady=false) tick reports no event");
        psmCheck(plant.divertingWaitTicks == 0, "the pause resets the counter instead of freezing it");

        diverter.resolve(DiverterFaultKind::Blocked);
        auto freshFirst = psm::advance(plant, diverter, true);
        psmCheck(freshFirst.has_value() && *freshFirst == SystemEventKind::DiverterNotReady,
                 "routing resumes with a full, fresh grace period -- DiverterNotReady again, not "
                 "RoutingDeadlineMissed");
    }

    // -- A settled diverter routes normally and keeps the counter at 0.
    {
        Plant plant;
        Diverter diverter;
        psm::spawnItem(plant, Item{1, Zone::Infeed, 750});

        for (int i = 0; i < 3; ++i) {
            diverter.resolve();
            psm::advance(plant, diverter, true);
        }
        psmCheck(plant.item->zone == Zone::Diverting, "item reaches Diverting");
        psmCheck(diverter.isSettled(), "diverter was never asked to move, so it's already settled");

        auto event = psm::advance(plant, diverter, true);
        psmCheck(!event.has_value(), "a settled diverter produces no event");
        psmCheck(plant.item->zone == Zone::OutputLight, "and the item routes immediately");
        psmCheck(plant.divertingWaitTicks == 0, "counter stays at 0 for a diverter that was never blocked");
    }

    return 0;
}
