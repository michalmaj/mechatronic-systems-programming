#include <psm/estop_latch.hpp>

namespace psm {

EStopLatchState nextEStopLatchState(EStopLatchState previous, bool pressed, bool released, bool resetRequested) {
    // TODO (Misja 16: przycisk_awaryjny): pressed zawsze wygrywa -> Engaged. Z Engaged: released
    // przechodzi do Armed. Z Armed: dopiero resetRequested wraca do Released (jeśli released i
    // resetRequested są prawdziwe naraz, na razie tylko released się liczy -- reset dotyczy
    // wyłącznie stanu Armed, nie Engaged).
    (void)pressed;
    (void)released;
    (void)resetRequested;
    return previous;
}

}  // namespace psm
