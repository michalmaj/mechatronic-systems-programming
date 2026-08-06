#include "support/check.hpp"
#include <psm/diverter.hpp>
#include <psm/plant.hpp>

int main() {
    // -- Original R0 sequence, now driven through a Diverter commanded early enough to settle --
    psm::Plant plant;
    psm::Diverter diverter;
    psmCheck(!plant.currentItem().has_value(), "plant starts empty");

    plant.spawnItem(psm::Item{1, psm::Zone::Infeed, 750});
    psmCheck(plant.currentItem().has_value(), "item present after spawn");
    psmCheck(plant.currentItem()->zone == psm::Zone::Infeed, "spawned item starts at Infeed");

    plant.spawnItem(psm::Item{2, psm::Zone::Infeed, 100});
    psmCheck(plant.currentItem()->id == 1, "spawning while occupied is ignored in R0");

    diverter.setCommand(psm::DiverterCommand::Divert);  // decided early, well before Diverting

    plant.advance(diverter);
    psmCheck(plant.currentItem()->zone == psm::Zone::PresenceCheck, "advances to PresenceCheck");
    diverter.resolve();  // Straight -> Moving

    plant.advance(diverter);
    psmCheck(plant.currentItem()->zone == psm::Zone::Weighing, "advances to Weighing");
    diverter.resolve();  // Moving -> Diverted (settled with a tick to spare)

    plant.advance(diverter);
    psmCheck(plant.currentItem()->zone == psm::Zone::Diverting, "advances to Diverting");
    diverter.resolve();  // already at target, no-op

    plant.advance(diverter);
    psmCheck(plant.currentItem()->zone == psm::Zone::OutputHeavy,
             "settled Diverted position routes to OutputHeavy");

    plant.advance(diverter);
    psmCheck(!plant.currentItem().has_value(), "item clears one tick after reaching an output zone");

    // -- New R1 scenario: command issued late, diverter still Moving when the item arrives --
    psm::Plant lateePlant;
    psm::Diverter lateDiverter;
    lateePlant.spawnItem(psm::Item{3, psm::Zone::Infeed, 750});

    lateePlant.advance(lateDiverter);
    lateDiverter.resolve();
    psmCheck(lateePlant.currentItem()->zone == psm::Zone::PresenceCheck, "late scenario: reaches PresenceCheck");

    lateePlant.advance(lateDiverter);
    lateDiverter.resolve();
    psmCheck(lateePlant.currentItem()->zone == psm::Zone::Weighing, "late scenario: reaches Weighing");

    lateDiverter.setCommand(psm::DiverterCommand::Divert);  // decided late, right as item leaves Weighing
    lateePlant.advance(lateDiverter);
    psmCheck(lateePlant.currentItem()->zone == psm::Zone::Diverting, "late scenario: reaches Diverting");
    lateDiverter.resolve();
    psmCheck(lateDiverter.actualPosition() == psm::DiverterPosition::Moving,
             "late scenario: diverter begins moving toward Diverted");

    lateePlant.advance(lateDiverter);
    psmCheck(lateePlant.currentItem()->zone == psm::Zone::Diverting,
             "late scenario: item waits at Diverting while diverter is Moving");
    lateDiverter.resolve();
    psmCheck(lateDiverter.actualPosition() == psm::DiverterPosition::Diverted,
             "late scenario: diverter arrives at Diverted");

    lateePlant.advance(lateDiverter);
    psmCheck(lateePlant.currentItem()->zone == psm::Zone::OutputHeavy,
             "late scenario: item routes once diverter finally settles");

    return 0;
}
