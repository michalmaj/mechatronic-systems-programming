#include <psm/mode.hpp>

namespace psm {

Mode modeStep(Mode current, bool startRequested, bool stopRequested, EStopLatchState latch, bool resetRequested) {
    if (latch != EStopLatchState::Released) {
        return Mode::EStopped;
    }
    if (current == Mode::EStopped) {
        return Mode::Idle;
    }
    // TODO (Misja 27: tryb_awarii): current == Fault musi być pełnym "wyłapywaczem", sprawdzanym
    // TU -- przed stopRequested/startRequested poniżej -- resetRequested ? Idle : Fault.
    (void)resetRequested;
    if (stopRequested) {
        return Mode::Idle;
    }
    if (startRequested) {
        return Mode::Running;
    }
    return current;
}

Mode reactToSystemEvent(Mode modeForTick, std::optional<SystemEventKind> event) {
    // TODO (Misja 27: tryb_awarii): jeśli modeForTick == Running i event == RoutingDeadlineMissed,
    // zwróć Fault. W przeciwnym razie zwróć modeForTick bez zmian. To cała reguła.
    (void)event;
    return modeForTick;
}

}  // namespace psm
