#include <psm/estop_latch.hpp>

namespace psm {

EStopLatchState nextEStopLatchState(EStopLatchState previous, bool pressed, bool released, bool resetRequested) {
    if (pressed) {
        return EStopLatchState::Engaged;
    }
    if (previous == EStopLatchState::Engaged) {
        return released ? EStopLatchState::Armed : EStopLatchState::Engaged;
    }
    if (previous == EStopLatchState::Armed) {
        return resetRequested ? EStopLatchState::Released : EStopLatchState::Armed;
    }
    return EStopLatchState::Released;
}

}  // namespace psm
