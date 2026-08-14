#include <iostream>

#include <psm/engine.hpp>
#include <psm/scenario.hpp>

#include "scenario_demos.hpp"

namespace {

void runAndPrint(const char* title, const psm::Scenario& scenario) {
    std::cout << "-- " << title << " --\n";
    auto trace = psm::runScenario(scenario);
    if (!trace.has_value()) {
        std::cout << "scenario did not complete\n";
        return;
    }
    for (const auto& result : *trace) {
        std::cout << psm::describe(result) << '\n';
    }
}

}  // namespace

int main() {
    runAndPrint("Scripted diverter-fault recovery scenario", recoveryDemoScenario());
    std::cout << '\n';
    runAndPrint("Scripted multi-parcel scenario", multiParcelDemoScenario());
    return 0;
}
