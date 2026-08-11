#include <psm/tick_result.hpp>

namespace psm {

std::string describe(const TickResult& result) {
    // TODO (Misja 32: silnik_z_wieloma_paczkami): dokładny, wymagany format (jedna linia):
    // "tick <N>: mode=<M> belt=<B> latch=<L> diverter=<cmd>/<pos>@<id|-> event=<e|->
    //  infeed=<id|-> presenceCheck=<id|-> weighing=<id|-> diverting=<id|->
    //  departure=<id->dest|->"
    // gdzie <M> to nazwa enumeratora Mode ("Idle"/"Running"/"EStopped"/"Fault"), <B> nazwa
    // enumeratora BeltMotorState, <L> nazwa enumeratora EStopLatchState, <cmd> nazwa enumeratora
    // DiverterCommand ("HoldStraight"/"Divert"), <pos> nazwa enumeratora DiverterPosition, <id>
    // dowolny ItemId lub "-", <e> nazwa enumeratora SystemEventKind lub "-", <dest> "Light" dla
    // Zone::OutputLight albo "Heavy" dla Zone::OutputHeavy. Zobacz materiały misji po dokładne
    // przykłady.
    (void)result;
    return "";
}

}  // namespace psm
