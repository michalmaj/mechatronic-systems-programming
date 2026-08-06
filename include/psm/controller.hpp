#pragma once

#include <psm/diverter_command.hpp>
#include <psm/item.hpp>
#include <psm/weight_class.hpp>

namespace psm {

WeightClass classify(Grams mass);
DiverterCommand toDiverterCommand(WeightClass weightClass);

}  // namespace psm
