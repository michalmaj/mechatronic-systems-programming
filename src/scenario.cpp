#include <psm/scenario.hpp>

#include <psm/engine.hpp>

namespace psm {

bool isValidScenario(const Scenario& scenario) {
    for (const auto& fault : scenario.sensorFaults) {
        if (!(fault.from < fault.until) || fault.until > scenario.duration) {
            return false;
        }
    }
    for (const auto& fault : scenario.diverterFaults) {
        if (!(fault.from < fault.until) || fault.until > scenario.duration) {
            return false;
        }
    }

    for (std::size_t i = 0; i < scenario.sensorFaults.size(); ++i) {
        for (std::size_t j = i + 1; j < scenario.sensorFaults.size(); ++j) {
            const auto& a = scenario.sensorFaults[i];
            const auto& b = scenario.sensorFaults[j];
            if (a.target == b.target && a.from < b.until && b.from < a.until) {
                return false;
            }
        }
    }
    for (std::size_t i = 0; i < scenario.diverterFaults.size(); ++i) {
        for (std::size_t j = i + 1; j < scenario.diverterFaults.size(); ++j) {
            const auto& a = scenario.diverterFaults[i];
            const auto& b = scenario.diverterFaults[j];
            if (a.from < b.until && b.from < a.until) {
                return false;
            }
        }
    }

    for (const auto& input : scenario.operatorInputs) {
        if (input.at >= scenario.duration) {
            return false;
        }
    }
    for (std::size_t i = 0; i < scenario.operatorInputs.size(); ++i) {
        for (std::size_t j = i + 1; j < scenario.operatorInputs.size(); ++j) {
            const auto& a = scenario.operatorInputs[i];
            const auto& b = scenario.operatorInputs[j];
            if (a.at != b.at) {
                continue;
            }
            if (a.kind == b.kind) {
                return false;
            }
            const bool isAllowedPair =
                (a.kind == ScenarioInputKind::EmergencyStopReleased && b.kind == ScenarioInputKind::Reset) ||
                (a.kind == ScenarioInputKind::Reset && b.kind == ScenarioInputKind::EmergencyStopReleased);
            if (!isAllowedPair) {
                return false;
            }
        }
    }

    for (const auto& arrival : scenario.arrivals) {
        if (arrival.at >= scenario.duration) {
            return false;
        }
    }
    for (std::size_t i = 0; i < scenario.arrivals.size(); ++i) {
        for (std::size_t j = i + 1; j < scenario.arrivals.size(); ++j) {
            if (scenario.arrivals[i].at == scenario.arrivals[j].at) {
                return false;
            }
            if (scenario.arrivals[i].id == scenario.arrivals[j].id) {
                return false;
            }
        }
    }

    return true;
}

namespace {

std::optional<SensorFaultKind> activeSensorFault(SensorTarget target, Tick now,
                                                  const std::vector<ScriptedSensorFault>& faults) {
    for (const auto& fault : faults) {
        if (fault.target == target && fault.from <= now && now < fault.until) {
            return fault.kind;
        }
    }
    return std::nullopt;
}

std::optional<DiverterFaultKind> activeDiverterFault(Tick now, const std::vector<ScriptedDiverterFault>& faults) {
    for (const auto& fault : faults) {
        if (fault.from <= now && now < fault.until) {
            return fault.kind;
        }
    }
    return std::nullopt;
}

}  // namespace

std::optional<std::vector<TickResult>> runScenario(const Scenario& scenario) {
    if (!isValidScenario(scenario)) {
        return std::nullopt;
    }

    Engine engine;
    std::vector<TickResult> trace;
    trace.reserve(scenario.duration);

    for (Tick tick = 0; tick < scenario.duration; ++tick) {
        for (const auto& arrival : scenario.arrivals) {
            if (arrival.at == tick) {
                if (!engine.spawnItem(arrival.id, arrival.mass)) {
                    return std::nullopt;
                }
            }
        }

        for (const auto& input : scenario.operatorInputs) {
            if (input.at != tick) {
                continue;
            }
            switch (input.kind) {
                case ScenarioInputKind::EmergencyStopPressed: engine.requestEStop(); break;
                case ScenarioInputKind::EmergencyStopReleased: engine.releaseEStop(); break;
                case ScenarioInputKind::Reset: engine.requestReset(); break;
                case ScenarioInputKind::StartRequested: engine.requestStart(); break;
                case ScenarioInputKind::StopRequested: engine.requestStop(); break;
            }
        }

        if (auto kind = activeSensorFault(SensorTarget::Presence, tick, scenario.sensorFaults)) {
            engine.injectSensorFault(SensorTarget::Presence, *kind);
        } else {
            engine.clearSensorFault(SensorTarget::Presence);
        }
        if (auto kind = activeSensorFault(SensorTarget::Weight, tick, scenario.sensorFaults)) {
            engine.injectSensorFault(SensorTarget::Weight, *kind);
        } else {
            engine.clearSensorFault(SensorTarget::Weight);
        }
        if (auto kind = activeDiverterFault(tick, scenario.diverterFaults)) {
            engine.injectDiverterFault(*kind);
        } else {
            engine.clearDiverterFault();
        }

        trace.push_back(engine.step());
    }

    return trace;
}

}  // namespace psm
