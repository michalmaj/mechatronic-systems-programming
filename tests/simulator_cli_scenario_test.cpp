#include "support/check.hpp"
#include <psm/engine.hpp>
#include <psm/scenario.hpp>

#include "scenario_demos.hpp"

int main() {
    using psm::Mode;

    auto recoveryTrace = psm::runScenario(recoveryDemoScenario());
    psmCheck(recoveryTrace.has_value(), "recoveryDemoScenario runs to completion");
    if (recoveryTrace.has_value()) {
        bool sawFault = false;
        for (const auto& result : *recoveryTrace) {
            if (result.mode == Mode::Fault) {
                sawFault = true;
            }
        }
        psmCheck(sawFault, "recovery scenario shows Mode::Fault at least once");
    }

    auto multiParcelTrace = psm::runScenario(multiParcelDemoScenario());
    psmCheck(multiParcelTrace.has_value(), "multiParcelDemoScenario runs to completion");
    if (multiParcelTrace.has_value()) {
        int departures = 0;
        for (const auto& result : *multiParcelTrace) {
            if (result.departure.has_value()) {
                ++departures;
            }
        }
        psmCheck(departures >= 3, "multi-parcel scenario produces at least three departures");
    }

    return 0;
}
