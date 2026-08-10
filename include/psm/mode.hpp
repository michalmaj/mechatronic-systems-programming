#pragma once

#include <optional>

#include <psm/estop_latch.hpp>
#include <psm/system_event_kind.hpp>

namespace psm {

enum class Mode { Idle, Running, EStopped, Fault };

Mode modeStep(Mode current, bool startRequested, bool stopRequested,
              EStopLatchState latch = EStopLatchState::Released,
              bool resetRequested = false);

Mode reactToSystemEvent(Mode modeForTick, std::optional<SystemEventKind> event);

}  // namespace psm
