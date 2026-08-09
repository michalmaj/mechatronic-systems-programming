#pragma once

namespace psm {

enum class Mode { Idle, Running };

Mode modeStep(Mode current, bool startRequested, bool stopRequested);

}  // namespace psm
