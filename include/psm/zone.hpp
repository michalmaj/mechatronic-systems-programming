#pragma once

#include <string_view>

namespace psm {

enum class Zone { Infeed, PresenceCheck, Weighing, Diverting, OutputLight, OutputHeavy };

std::string_view toString(Zone zone);

}  // namespace psm
