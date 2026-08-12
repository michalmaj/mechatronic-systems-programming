#include <psm/tick_result.hpp>

namespace psm {

namespace {

const char* modeName(Mode mode) {
    switch (mode) {
        case Mode::Idle: return "Idle";
        case Mode::Running: return "Running";
        case Mode::EStopped: return "EStopped";
        case Mode::Fault: return "Fault";
    }
    return "Unknown";
}

const char* beltStateName(BeltMotorState state) {
    switch (state) {
        case BeltMotorState::Stopped: return "Stopped";
        case BeltMotorState::RampingUp: return "RampingUp";
        case BeltMotorState::Running: return "Running";
        case BeltMotorState::RampingDown: return "RampingDown";
    }
    return "Unknown";
}

const char* latchName(EStopLatchState latch) {
    switch (latch) {
        case EStopLatchState::Released: return "Released";
        case EStopLatchState::Engaged: return "Engaged";
        case EStopLatchState::Armed: return "Armed";
    }
    return "Unknown";
}

const char* diverterCommandName(DiverterCommand command) {
    return command == DiverterCommand::HoldStraight ? "HoldStraight" : "Divert";
}

const char* diverterPositionName(DiverterPosition position) {
    switch (position) {
        case DiverterPosition::Straight: return "Straight";
        case DiverterPosition::Diverted: return "Diverted";
        case DiverterPosition::Moving: return "Moving";
    }
    return "Unknown";
}

const char* eventName(SystemEventKind kind) {
    return kind == SystemEventKind::DiverterNotReady ? "DiverterNotReady" : "RoutingDeadlineMissed";
}

const char* destinationName(Zone zone) {
    return zone == Zone::OutputLight ? "Light" : "Heavy";
}

std::string slot(const std::optional<Item>& item) {
    return item.has_value() ? std::to_string(item->id) : "-";
}

std::string idOrDash(std::optional<ItemId> id) {
    return id.has_value() ? std::to_string(*id) : "-";
}

}  // namespace

std::string describe(const TickResult& result) {
    return "tick " + std::to_string(result.tick) + ": mode=" + modeName(result.mode) +
           " belt=" + beltStateName(result.beltActual) + " latch=" + latchName(result.latch) +
           " diverter=" + diverterCommandName(result.diverterCommand) + "/" +
           diverterPositionName(result.diverterActual) + "@" + idOrDash(result.diverterCommandItemId) +
           " event=" + (result.event.has_value() ? eventName(*result.event) : "-") +
           " infeed=" + slot(result.infeed) + " presenceCheck=" + slot(result.presenceCheck) +
           " weighing=" + slot(result.weighing) + " diverting=" + slot(result.diverting) +
           " departure=" +
           (result.departure.has_value()
                ? std::to_string(result.departure->id) + "->" + destinationName(result.departure->destination)
                : "-");
}

}  // namespace psm
