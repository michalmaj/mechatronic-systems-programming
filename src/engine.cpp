#include <psm/engine.hpp>

#include <optional>

#include <psm/controller.hpp>

namespace psm {

void Engine::spawnItem(Item item) {
    // TODO (Misja 11: silnik_formalizuje_kolejnosc): wywołaj wolną funkcję
    // psm::spawnItem(plant_, item) -- Plant to wciąż zwykły struct, nie ma metody spawnItem.
    (void)item;
}

TickResult Engine::step() {
    // TODO (Misja 11: silnik_formalizuje_kolejnosc): w tej dokładnie kolejności:
    // 1) jeśli plant_.item ma wartość, policz DiverterCommand przez classify + toDiverterCommand;
    // 2) diverter_.setCommand(...);
    // 3) diverter_.resolve();
    // 4) psm::advance(plant_, diverter_) -- znowu wolna funkcja, nie metoda;
    // 5) złóż TickResult dla ticku tick_ (PRZED inkrementacją), potem zwiększ tick_.
    return TickResult{tick_, std::nullopt, DiverterCommand::HoldStraight, DiverterPosition::Straight};
}

}  // namespace psm
