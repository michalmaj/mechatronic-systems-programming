#include <psm/mode.hpp>

namespace psm {

Mode modeStep(Mode current, bool startRequested, bool stopRequested) {
    // TODO (Misja 14: tryb_pracy): jeśli stopRequested, wynik to Idle (ma pierwszeństwo nad
    // startRequested). W przeciwnym razie, jeśli startRequested, wynik to Running. W przeciwnym
    // razie zwróć current bez zmian.
    (void)startRequested;
    (void)stopRequested;
    return current;
}

}  // namespace psm
