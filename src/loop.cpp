#include <psm/loop.hpp>

#include <psm/controller.hpp>
#include <psm/diverter.hpp>

namespace psm {

void runTicks(Plant& plant, int tickCount) {
    // TODO (Misja 9: przenosnik_czeka_na_dywerter): dla każdego z tickCount ticków, w tej
    // kolejności: 1) jeśli plant.item ma wartość, policz WeightClass przez classify, a potem
    // DiverterCommand przez toDiverterCommand; 2) diverter.setCommand(...); 3) diverter.resolve();
    // 4) advance(plant, diverter). Potrzebujesz własnego, lokalnego obiektu Diverter w tej funkcji.
    (void)plant;
    (void)tickCount;
}

}  // namespace psm
