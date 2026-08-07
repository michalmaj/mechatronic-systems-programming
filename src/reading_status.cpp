#include <psm/reading_status.hpp>

namespace psm {

std::string_view toString(ReadingStatus status) {
    switch (status) {
        case ReadingStatus::Ok: return "Ok";
        case ReadingStatus::Missing: return "Missing";
        case ReadingStatus::Stale: return "Stale";
        case ReadingStatus::OutOfRange: return "OutOfRange";
    }
    return "Unknown";
}

}  // namespace psm
