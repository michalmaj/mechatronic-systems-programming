#include "support/check.hpp"
#include <psm/diverter.hpp>
#include <psm/plant.hpp>
#include <psm/system_event_kind.hpp>

int main() {
    using psm::Diverter;
    using psm::DiverterCommand;
    using psm::DiverterFaultKind;
    using psm::Plant;
    using psm::SystemEventKind;
    using psm::Zone;

    // -- A permanently blocked diverter: first attempt DiverterNotReady, second (consecutive)
    // attempt RoutingDeadlineMissed -- the frozen deadline timing.
    {
        Plant plant;
        Diverter diverter;
        psm::spawnItem(plant, 1, 750);
        diverter.setCommand(DiverterCommand::Divert);

        for (int i = 0; i < 3; ++i) {
            diverter.resolve(DiverterFaultKind::Blocked);
            psm::advance(plant, diverter, true);
        }
        psmCheck(plant.diverting.has_value() && plant.diverting->id == 1, "item reaches diverting");
        psmCheck(plant.diverting->divertingWaitTicks == 0, "counter starts at 0 the moment the item enters diverting");

        diverter.resolve(DiverterFaultKind::Blocked);
        auto first = psm::advance(plant, diverter, true);
        psmCheck(first.event.has_value() && *first.event == SystemEventKind::DiverterNotReady,
                 "first unsettled active attempt: DiverterNotReady");
        psmCheck(plant.diverting.has_value(), "item still waits");

        diverter.resolve(DiverterFaultKind::Blocked);
        auto second = psm::advance(plant, diverter, true);
        psmCheck(second.event.has_value() && *second.event == SystemEventKind::RoutingDeadlineMissed,
                 "second consecutive unsettled active attempt: RoutingDeadlineMissed");
        psmCheck(plant.diverting.has_value(), "item still waits -- advance() never routes on its own");
    }

    // -- !routingReady resets the counter instead of freezing it: an interrupted attempt gets a
    // full, fresh grace period once routing resumes.
    {
        Plant plant;
        Diverter diverter;
        psm::spawnItem(plant, 1, 750);
        diverter.setCommand(DiverterCommand::Divert);

        for (int i = 0; i < 3; ++i) {
            diverter.resolve(DiverterFaultKind::Blocked);
            psm::advance(plant, diverter, true);
        }
        psmCheck(plant.diverting.has_value(), "item reaches diverting");

        diverter.resolve(DiverterFaultKind::Blocked);
        auto notReady = psm::advance(plant, diverter, true);
        psmCheck(notReady.event.has_value() && *notReady.event == SystemEventKind::DiverterNotReady,
                 "one active unsettled attempt: DiverterNotReady, counter now at 1");

        diverter.resolve(DiverterFaultKind::Blocked);
        auto paused = psm::advance(plant, diverter, false);
        psmCheck(!paused.event.has_value(), "a paused (routingReady=false) tick reports no event");
        psmCheck(plant.diverting->divertingWaitTicks == 0, "the pause resets the counter instead of freezing it");

        diverter.resolve(DiverterFaultKind::Blocked);
        auto freshFirst = psm::advance(plant, diverter, true);
        psmCheck(freshFirst.event.has_value() && *freshFirst.event == SystemEventKind::DiverterNotReady,
                 "routing resumes with a full, fresh grace period -- DiverterNotReady again, not "
                 "RoutingDeadlineMissed");
    }

    // -- A settled diverter routes normally and keeps the counter at 0.
    {
        Plant plant;
        Diverter diverter;
        psm::spawnItem(plant, 1, 750);

        for (int i = 0; i < 3; ++i) {
            diverter.resolve();
            psm::advance(plant, diverter, true);
        }
        psmCheck(plant.diverting.has_value(), "item reaches diverting");
        psmCheck(diverter.isSettled(), "diverter was never asked to move, so it's already settled");

        auto result = psm::advance(plant, diverter, true);
        psmCheck(!result.event.has_value(), "a settled diverter produces no event");
        psmCheck(result.departure.has_value() && result.departure->id == 1 &&
                     result.departure->destination == Zone::OutputLight,
                 "and the item routes immediately, to OutputLight");
        psmCheck(!plant.diverting.has_value(), "diverting is empty immediately after departure");
    }

    return 0;
}
