#include "support/check.hpp"
#include <psm/tick_result.hpp>

int main() {
    using psm::BeltMotorState;
    using psm::DiverterCommand;
    using psm::DiverterPosition;
    using psm::EStopLatchState;
    using psm::Item;
    using psm::ItemDeparture;
    using psm::Mode;
    using psm::SystemEventKind;
    using psm::TickResult;
    using psm::Zone;

    TickResult full{
        .tick = 3,
        .presenceCheck = Item{3, 150},
        .weighing = Item{2, 800},
        .diverting = Item{1, 750},
        .diverterCommand = DiverterCommand::Divert,
        .diverterCommandItemId = 1,
        .diverterActual = DiverterPosition::Moving,
        .mode = Mode::Running,
        .beltActual = BeltMotorState::Running,
        .latch = EStopLatchState::Released,
        .event = SystemEventKind::DiverterNotReady,
    };
    psmCheck(psm::describe(full) ==
                 "tick 3: mode=Running belt=Running latch=Released diverter=Divert/Moving@1 "
                 "event=DiverterNotReady infeed=- presenceCheck=3 weighing=2 diverting=1 departure=-",
             "describes a busy tick with three parcels resident and an active event");

    TickResult empty{
        .tick = 5,
        .diverterCommand = DiverterCommand::HoldStraight,
        .diverterActual = DiverterPosition::Straight,
        .mode = Mode::Idle,
        .beltActual = BeltMotorState::Stopped,
        .latch = EStopLatchState::Released,
    };
    psmCheck(psm::describe(empty) ==
                 "tick 5: mode=Idle belt=Stopped latch=Released diverter=HoldStraight/Straight@- "
                 "event=- infeed=- presenceCheck=- weighing=- diverting=- departure=-",
             "describes an empty, idle tick with no diverter command target");

    TickResult departed{
        .tick = 7,
        .departure = ItemDeparture{1, Zone::OutputLight},
        .diverterCommand = DiverterCommand::HoldStraight,
        .diverterActual = DiverterPosition::Straight,
        .mode = Mode::Running,
        .beltActual = BeltMotorState::Running,
        .latch = EStopLatchState::Released,
    };
    psmCheck(psm::describe(departed) ==
                 "tick 7: mode=Running belt=Running latch=Released diverter=HoldStraight/Straight@- "
                 "event=- infeed=- presenceCheck=- weighing=- diverting=- departure=1->Light",
             "reports a parcel's departure the same tick it happens");

    return 0;
}
