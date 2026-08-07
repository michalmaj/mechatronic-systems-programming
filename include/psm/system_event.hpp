#pragma once

#include <string_view>

#include <psm/tick.hpp>

namespace psm {

enum class SystemEventKind { DiverterNotReady, RoutingDeadlineMissed };

struct SystemEvent {
    Tick at;
    SystemEventKind kind;
};

std::string_view toString(SystemEventKind kind);

}  // namespace psm
