#pragma once

#include <string_view>

namespace psm {

enum class ReadingStatus { Ok, Missing, Stale, OutOfRange };

std::string_view toString(ReadingStatus status);

}  // namespace psm
