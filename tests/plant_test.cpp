#include "support/check.hpp"
#include <psm/diverter.hpp>
#include <psm/plant.hpp>

namespace {

psm::BeltMotor runningBeltMotor() {
    psm::BeltMotor motor;
    motor.setCommand(psm::BeltMotorCommand::Run);
    motor.resolve();
    motor.resolve();
    return motor;
}

}  // namespace

int main() {
    // -- Original R0 sequence, now driven through a Diverter commanded early enough to settle,
    //    with the belt running throughout --
    psm::Plant plant;
    psm::Diverter diverter;
    psm::BeltMotor belt = runningBeltMotor();
    psmCheck(!plant.currentItem().has_value(), "plant starts empty");

    plant.spawnItem(psm::Item{1, psm::Zone::Infeed, 750});
    psmCheck(plant.currentItem().has_value(), "item present after spawn");
    psmCheck(plant.currentItem()->zone == psm::Zone::Infeed, "spawned item starts at Infeed");

    plant.spawnItem(psm::Item{2, psm::Zone::Infeed, 100});
    psmCheck(plant.currentItem()->id == 1, "spawning while occupied is ignored in R0");

    diverter.setCommand(psm::DiverterCommand::Divert);

    plant.advance(diverter, belt);
    psmCheck(plant.currentItem()->zone == psm::Zone::PresenceCheck, "advances to PresenceCheck");
    diverter.resolve();

    plant.advance(diverter, belt);
    psmCheck(plant.currentItem()->zone == psm::Zone::Weighing, "advances to Weighing");
    diverter.resolve();

    plant.advance(diverter, belt);
    psmCheck(plant.currentItem()->zone == psm::Zone::Diverting, "advances to Diverting");
    diverter.resolve();

    plant.advance(diverter, belt);
    psmCheck(plant.currentItem()->zone == psm::Zone::OutputHeavy,
             "settled Diverted position routes to OutputHeavy");

    plant.advance(diverter, belt);
    psmCheck(!plant.currentItem().has_value(), "item clears one tick after reaching an output zone");

    // -- Late-command scenario from R1, still valid with the belt running --
    psm::Plant lateePlant;
    psm::Diverter lateDiverter;
    psm::BeltMotor lateBelt = runningBeltMotor();
    lateePlant.spawnItem(psm::Item{3, psm::Zone::Infeed, 750});

    lateePlant.advance(lateDiverter, lateBelt);
    lateDiverter.resolve();
    psmCheck(lateePlant.currentItem()->zone == psm::Zone::PresenceCheck, "late scenario: reaches PresenceCheck");

    lateePlant.advance(lateDiverter, lateBelt);
    lateDiverter.resolve();
    psmCheck(lateePlant.currentItem()->zone == psm::Zone::Weighing, "late scenario: reaches Weighing");

    lateDiverter.setCommand(psm::DiverterCommand::Divert);
    lateePlant.advance(lateDiverter, lateBelt);
    psmCheck(lateePlant.currentItem()->zone == psm::Zone::Diverting, "late scenario: reaches Diverting");
    lateDiverter.resolve();
    psmCheck(lateDiverter.actualPosition() == psm::DiverterPosition::Moving,
             "late scenario: diverter begins moving toward Diverted");

    lateePlant.advance(lateDiverter, lateBelt);
    psmCheck(lateePlant.currentItem()->zone == psm::Zone::Diverting,
             "late scenario: item waits at Diverting while diverter is Moving");
    lateDiverter.resolve();
    psmCheck(lateDiverter.actualPosition() == psm::DiverterPosition::Diverted,
             "late scenario: diverter arrives at Diverted");

    lateePlant.advance(lateDiverter, lateBelt);
    psmCheck(lateePlant.currentItem()->zone == psm::Zone::OutputHeavy,
             "late scenario: item routes once diverter finally settles");

    // -- New R3b scenario: belt not running means nothing moves, regardless of diverter state --
    psm::Plant stoppedPlant;
    psm::Diverter stoppedDiverter;
    psm::BeltMotor stoppedBelt;  // default-constructed: Stopped
    stoppedPlant.spawnItem(psm::Item{4, psm::Zone::Infeed, 100});

    stoppedPlant.advance(stoppedDiverter, stoppedBelt);
    psmCheck(stoppedPlant.currentItem()->zone == psm::Zone::Infeed,
             "item does not move while the belt is Stopped");

    stoppedPlant.advance(stoppedDiverter, stoppedBelt);
    psmCheck(stoppedPlant.currentItem()->zone == psm::Zone::Infeed,
             "item still does not move on a second tick with the belt Stopped");

    return 0;
}
