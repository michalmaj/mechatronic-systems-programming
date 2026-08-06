#include "support/check.hpp"
#include <psm/diverter_position.hpp>
#include <psm/item.hpp>
#include <psm/weight_class.hpp>

int main() {
    psm::Item item{1, psm::Zone::Infeed, 750};
    psmCheck(item.id == 1, "Item stores its id");
    psmCheck(item.zone == psm::Zone::Infeed, "Item stores its zone");
    psmCheck(item.mass == 750, "Item stores its mass");

    psmCheck(psm::WeightClass::Light != psm::WeightClass::Heavy, "WeightClass values are distinct");
    psmCheck(psm::DiverterPosition::Straight != psm::DiverterPosition::Diverted,
             "DiverterPosition values are distinct");

    return 0;
}
