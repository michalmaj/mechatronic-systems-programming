#include "support/check.hpp"
#include <psm/engine.hpp>

int main() {
    using psm::DiverterFaultKind;
    using psm::Mode;
    using psm::SystemEventKind;
    using psm::Zone;

    psm::Engine engine;
    engine.spawnItem(psm::Item{1, Zone::Infeed, 750});
    engine.requestStart();
    engine.injectDiverterFault(DiverterFaultKind::Blocked);

    auto result = engine.step();
    psmCheck(result.mode == Mode::Running, "requestStart takes effect the very next step, same as every prior module");
    psmCheck(result.beltActual == psm::BeltMotorState::RampingUp, "belt begins ramping up on that same step");
    psmCheck(result.item->zone == Zone::Infeed, "item still waits -- belt is not Running yet");

    result = engine.step();
    psmCheck(result.beltActual == psm::BeltMotorState::Running, "belt finishes ramping up");
    psmCheck(result.item->zone == Zone::PresenceCheck, "belt is now Running: the item advances exactly one step");

    result = engine.step();
    psmCheck(result.item->zone == Zone::Weighing, "next step: PresenceCheck -> Weighing");

    result = engine.step();
    psmCheck(result.item->zone == Zone::Diverting, "next step: Weighing -> Diverting, classification now cached");
    psmCheck(!result.event.has_value(), "arriving at Diverting is not itself an event");

    result = engine.step();
    psmCheck(result.event.has_value() && *result.event == SystemEventKind::DiverterNotReady,
             "first active routing attempt against a Blocked diverter: DiverterNotReady");
    psmCheck(result.mode == Mode::Running, "not a deadline miss yet: mode stays Running");
    psmCheck(result.item->zone == Zone::Diverting, "item still waits");

    result = engine.step();
    psmCheck(result.event.has_value() && *result.event == SystemEventKind::RoutingDeadlineMissed,
             "second consecutive unsettled active attempt: RoutingDeadlineMissed");
    psmCheck(result.mode == Mode::Fault, "and Mode::Fault, in the very same TickResult");
    psmCheck(result.beltActual == psm::BeltMotorState::Running,
             "the belt is still Running this tick -- it was gated by Running before Fault was detected "
             "(the accepted one-tick delay, unlike e-stop's immediate forceStop())");

    result = engine.step();
    psmCheck(result.mode == Mode::Fault, "Fault persists with no reset requested");
    psmCheck(result.beltActual == psm::BeltMotorState::RampingDown,
             "only now, one tick after Mode::Fault first appeared, does the belt begin ramping down");

    result = engine.step();
    psmCheck(result.beltActual == psm::BeltMotorState::Stopped, "belt finishes ramping down");
    psmCheck(result.mode == Mode::Fault, "still Fault -- nothing has cleared or reset it yet");

    engine.clearDiverterFault();
    result = engine.step();
    psmCheck(result.mode == Mode::Fault,
             "clearDiverterFault() alone does not leave Fault -- it clears the cause, not the latch");

    engine.requestReset();
    result = engine.step();
    psmCheck(result.mode == Mode::Idle, "requestReset() now clears the latch: Fault -> Idle");
    psmCheck(result.item->zone == Zone::Diverting, "the parcel is still exactly where it was frozen");

    engine.requestStart();
    result = engine.step();
    psmCheck(result.mode == Mode::Running, "requestStart() resumes -- recovery always goes through Idle first");
    psmCheck(result.beltActual == psm::BeltMotorState::RampingUp,
             "the belt was fully Stopped during Fault, so it needs to ramp up again from scratch");
    psmCheck(result.item->zone == Zone::Diverting,
             "the item still waits -- the belt is not Running yet, so advance() has not run this tick");

    result = engine.step();
    psmCheck(result.beltActual == psm::BeltMotorState::Running, "belt finishes ramping up again");
    psmCheck(!result.event.has_value(),
             "by the time advance() runs again, the diverter (fault cleared, moving since the previous "
             "step) has already settled -- no event at all, straight to a routed parcel");
    psmCheck(result.item->zone == Zone::OutputHeavy,
             "and the very same parcel that was stuck finally routes -- full recovery");

    return 0;
}
