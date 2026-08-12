#include <psm/scenario.hpp>

namespace psm {

bool isValidScenario(const Scenario& scenario) {
    // TODO (Misja 33: model_i_walidacja): pełny zestaw reguł -- patrz materiały misji. Ten stub
    // celowo odrzuca wszystko.
    (void)scenario;
    return false;
}

std::optional<std::vector<TickResult>> runScenario(const Scenario& scenario) {
    // TODO (Misja 34: odtwarzacz): pełny algorytm replay -- patrz materiały misji. Ten stub celowo
    // nigdy się nie kończy powodzeniem.
    (void)scenario;
    return std::nullopt;
}

}  // namespace psm
