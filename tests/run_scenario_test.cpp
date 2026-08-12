#include "support/check.hpp"
#include <psm/engine.hpp>
#include <psm/scenario.hpp>

#include <cstddef>
#include <vector>

int main() {
    using psm::DiverterFaultKind;
    using psm::Engine;
    using psm::EStopLatchState;
    using psm::Scenario;
    using psm::ScenarioInputKind;
    using psm::TickResult;

    // -- Equivalence: the same short call sequence, driven imperatively and via a Scenario, must
    // produce identical traces, field by field.
    {
        Engine engine;
        engine.spawnItem(1, 750);
        engine.requestStart();
        std::vector<TickResult> imperative;
        for (int i = 0; i < 8; ++i) {
            imperative.push_back(engine.step());
        }

        Scenario scenario{};
        scenario.arrivals = {{0, 1, 750}};
        scenario.operatorInputs = {{0, ScenarioInputKind::StartRequested}};
        scenario.duration = 8;
        auto scripted = psm::runScenario(scenario);

        psmCheck(scripted.has_value(), "a valid, completable scenario runs to completion");
        psmCheck(scripted->size() == imperative.size(), "same number of ticks");
        bool allMatch = true;
        for (std::size_t i = 0; i < imperative.size() && i < scripted->size(); ++i) {
            const auto& a = imperative[i];
            const auto& b = (*scripted)[i];
            if (a.tick != b.tick || a.mode != b.mode || a.beltActual != b.beltActual || a.latch != b.latch ||
                a.diverterCommand != b.diverterCommand || a.diverterActual != b.diverterActual ||
                a.infeed.has_value() != b.infeed.has_value() ||
                a.presenceCheck.has_value() != b.presenceCheck.has_value() ||
                a.weighing.has_value() != b.weighing.has_value() ||
                a.diverting.has_value() != b.diverting.has_value() ||
                a.departure.has_value() != b.departure.has_value() || a.event != b.event) {
                allMatch = false;
                break;
            }
        }
        psmCheck(allMatch, "the scripted trace matches the imperative trace tick by tick, field by field");
    }

    // -- {EmergencyStopReleased, Reset} same-tick: latch reaches Armed, not Released, that tick.
    {
        Scenario scenario{};
        scenario.arrivals = {{0, 1, 750}};
        scenario.operatorInputs = {{0, ScenarioInputKind::StartRequested},
                                    {3, ScenarioInputKind::EmergencyStopPressed},
                                    {5, ScenarioInputKind::EmergencyStopReleased},
                                    {5, ScenarioInputKind::Reset}};
        scenario.duration = 6;
        auto trace = psm::runScenario(scenario);
        psmCheck(trace.has_value(), "scenario with the one allowed same-tick pair runs to completion");
        psmCheck((*trace)[5].latch == EStopLatchState::Armed,
                 "EmergencyStopReleased+Reset at the same tick reaches Armed, not Released -- reset alone "
                 "cannot skip the Engaged->Armed stage");
    }

    // -- A scheduled arrival against an occupied Infeed is a scenario error: runScenario returns nullopt.
    {
        Scenario scenario{};
        scenario.arrivals = {{0, 1, 750}, {3, 2, 100}};
        scenario.duration = 5;
        auto trace = psm::runScenario(scenario);
        psmCheck(!trace.has_value(), "an arrival against a still-occupied Infeed makes runScenario return nullopt");
    }

    // -- A statically-invalid scenario never runs at all.
    {
        Scenario scenario{};
        scenario.diverterFaults = {{0, 5, DiverterFaultKind::Blocked}, {3, 8, DiverterFaultKind::Blocked}};
        scenario.duration = 10;
        psmCheck(!psm::runScenario(scenario).has_value(),
                 "a statically invalid scenario makes runScenario return nullopt without running anything");
    }

    // -- duration == 0 is valid and produces an empty, but present, trace.
    {
        Scenario scenario{};
        scenario.duration = 0;
        auto trace = psm::runScenario(scenario);
        psmCheck(trace.has_value(), "a zero-duration scenario runs to completion");
        psmCheck(trace->empty(), "and produces an empty trace");
    }

    // -- Permutation invariance: reordering entries within each vector, at/from/until unchanged,
    // does not change runScenario's output.
    {
        Scenario original{};
        original.arrivals = {{0, 1, 100}, {2, 2, 800}, {4, 3, 150}};
        original.operatorInputs = {{0, ScenarioInputKind::StartRequested}};
        original.duration = 20;

        Scenario reordered{};
        reordered.arrivals = {{4, 3, 150}, {0, 1, 100}, {2, 2, 800}};
        reordered.operatorInputs = {{0, ScenarioInputKind::StartRequested}};
        reordered.duration = 20;

        auto originalTrace = psm::runScenario(original);
        auto reorderedTrace = psm::runScenario(reordered);
        psmCheck(originalTrace.has_value() && reorderedTrace.has_value(), "both orderings run to completion");
        psmCheck(originalTrace->size() == reorderedTrace->size(), "same trace length");
        bool allMatch = true;
        for (std::size_t i = 0; i < originalTrace->size(); ++i) {
            const auto& a = (*originalTrace)[i];
            const auto& b = (*reorderedTrace)[i];
            if (a.infeed.has_value() != b.infeed.has_value() ||
                a.presenceCheck.has_value() != b.presenceCheck.has_value() ||
                a.weighing.has_value() != b.weighing.has_value() ||
                a.diverting.has_value() != b.diverting.has_value() ||
                a.departure.has_value() != b.departure.has_value() ||
                (a.departure.has_value() && a.departure->id != b.departure->id)) {
                allMatch = false;
                break;
            }
        }
        psmCheck(allMatch, "reordering vector entries (same at/from/until/id/mass values) does not change the trace");
    }

    return 0;
}
