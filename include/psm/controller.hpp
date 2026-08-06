#pragma once

#include <psm/diverter_position.hpp>
#include <psm/item.hpp>
#include <psm/weight_class.hpp>

namespace psm {

WeightClass classify(Grams mass);
DiverterPosition toDiverterPosition(WeightClass weightClass);

}  // namespace psm
