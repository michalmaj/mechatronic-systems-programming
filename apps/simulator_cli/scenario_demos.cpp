#include "scenario_demos.hpp"

psm::Scenario recoveryDemoScenario() {
    // TODO (Misja 35: integracja_w_cli): odtwórz ten sam mechanizm i oś czasu co ślad odzyskiwania z
    // Modułu 7 (course/module_07_fault_mode/04_silnik_z_wykrywaniem_awarii.md) -- jedno przybycie
    // (750g) na ticku 0, StartRequested na ticku 0, ScriptedDiverterFault{Blocked} aktywny od ticku 0
    // do ticku 8, Reset na ticku 9, StartRequested na ticku 10, duration=12.
    return psm::Scenario{};
}

psm::Scenario multiParcelDemoScenario() {
    // TODO (Misja 35: integracja_w_cli): trzy przybycia o różnych klasyfikacjach (np. Light/Heavy/
    // Light -- 100g/800g/150g), odstępy między tickami przybyć co najmniej 2, żeby żadne nie trafiło
    // na wciąż zajęty Infeed.
    return psm::Scenario{};
}
