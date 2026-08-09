#pragma once

namespace psm {

enum class EStopLatchState { Released, Engaged, Armed };

EStopLatchState nextEStopLatchState(EStopLatchState previous, bool pressed, bool released, bool resetRequested);

}  // namespace psm
