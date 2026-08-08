#include <psm/diverter.hpp>

namespace psm {

void Diverter::setCommand(DiverterCommand command) {
    // TODO (Misja 8: dywerter_jako_klasa): zapisz command w polu command_.
    (void)command;
}

void Diverter::resolve() {
    // TODO (Misja 8: dywerter_jako_klasa): zgodnie z tabelą przejść z treści misji, przesuń
    // actual_ o jeden krok w stronę pozycji docelowej wyznaczonej przez command_.
}

DiverterPosition Diverter::actualPosition() const {
    // TODO (Misja 8: dywerter_jako_klasa): zwróć actual_.
    return DiverterPosition::Straight;
}

bool Diverter::isSettled() const {
    // TODO (Misja 8: dywerter_jako_klasa): zwróć, czy actual_ odpowiada aktualnej pozycji
    // docelowej wyznaczonej przez command_ -- żywe porównanie, nie zapamiętany wynik.
    return true;
}

}  // namespace psm
