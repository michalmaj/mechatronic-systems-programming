#include <psm/safety_supervisor.hpp>

namespace psm {

SafetyDecision checkEmergencyOverride(EStopLatchState latch) {
    // TODO (Misja 18: dwie_niezalezne_sciezki): zwróć SafetyDecision{true}, jeśli
    // latch != EStopLatchState::Released, w przeciwnym razie SafetyDecision{false}.
    (void)latch;
    return SafetyDecision{false};
}

bool diverterMayMove(Mode mode) {
    // TODO (Misja 18: dwie_niezalezne_sciezki): zwróć true wyłącznie dla Mode::Running.
    (void)mode;
    return false;
}

}  // namespace psm
