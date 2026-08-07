#pragma once

#include <psm/tick.hpp>

namespace psm {

enum class SystemEventKind { DiverterNotReady, RoutingDeadlineMissed };

struct SystemEvent {
    Tick at;
    SystemEventKind kind;
};

}  // namespace psm
