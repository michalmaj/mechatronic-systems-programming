#include <iostream>

#include <psm/engine.hpp>
#include <psm/scenario.hpp>

#include "scenario_demos.hpp"

int main() {
    bool ok = true;

    std::cout << "-- Scripted diverter-fault recovery scenario --\n";
    auto recoveryTrace = psm::runScenario(recoveryDemoScenario());
    if (!recoveryTrace.has_value()) {
        std::cout << "scenario did not complete\n";
        ok = false;
    } else {
        bool sawFault = false;
        for (const auto& result : *recoveryTrace) {
            std::cout << psm::describe(result) << '\n';
            if (result.mode == psm::Mode::Fault) {
                sawFault = true;
            }
        }
        if (!sawFault) {
            std::cout << "self-check failed: recovery scenario never showed Mode::Fault\n";
            ok = false;
        }
    }

    std::cout << "\n-- Scripted multi-parcel scenario --\n";
    auto multiParcelTrace = psm::runScenario(multiParcelDemoScenario());
    if (!multiParcelTrace.has_value()) {
        std::cout << "scenario did not complete\n";
        ok = false;
    } else {
        int departures = 0;
        for (const auto& result : *multiParcelTrace) {
            std::cout << psm::describe(result) << '\n';
            if (result.departure.has_value()) {
                ++departures;
            }
        }
        if (departures < 3) {
            std::cout << "self-check failed: multi-parcel scenario produced fewer than 3 departures\n";
            ok = false;
        }
    }

    return ok ? 0 : 1;
}
