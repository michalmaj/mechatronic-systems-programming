#include <psm/mode.hpp>

namespace psm {

Mode modeStep(Mode current, bool startRequested, bool stopRequested, EStopLatchState latch) {
    // TODO (Misja 17: tryb_zatrzymania_awaryjnego): e-stop ma absolutny priorytet -- jeśli
    // latch != EStopLatchState::Released, wynik to Mode::EStopped, sprawdzone PRZED wszystkim
    // poniżej.
    // TODO (Misja 17): jeśli current == Mode::EStopped (a powyższy warunek nie zadziałał, czyli
    // latch jest już Released), wynik to Mode::Idle -- powrót nigdy nie ląduje automatycznie
    // w Running, nawet jeśli startRequested jest prawdziwe w tym samym wywołaniu.
    (void)latch;
    if (stopRequested) {
        return Mode::Idle;
    }
    if (startRequested) {
        return Mode::Running;
    }
    return current;
}

}  // namespace psm
