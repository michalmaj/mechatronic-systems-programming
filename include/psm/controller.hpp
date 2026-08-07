#pragma once

#include <optional>

#include <psm/diverter_command.hpp>
#include <psm/item.hpp>
#include <psm/sensor_snapshot.hpp>
#include <psm/weight_class.hpp>

namespace psm {

WeightClass classify(Grams mass);
DiverterCommand toDiverterCommand(WeightClass weightClass);
std::optional<WeightClass> decideClassification(PresenceReading presence, WeightReading weight);

}  // namespace psm
