#pragma once

#include <psm/diverter_fault_kind.hpp>
#include <psm/tick.hpp>

namespace psm {

struct ScriptedDiverterFault {
    Tick from;
    Tick until;
    DiverterFaultKind kind;
};

}  // namespace psm
