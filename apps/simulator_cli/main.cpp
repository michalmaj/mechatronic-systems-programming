#include <iostream>

#include <psm/controller.hpp>
#include <psm/diverter.hpp>
#include <psm/plant.hpp>
#include <psm/zone.hpp>

int main() {
    // TODO (Misja 9: przenosnik_czeka_na_dywerter):
    // 1) stwórz Plant i Diverter, dodaj jedną paczkę przez spawnItem,
    // 2) w pętli (np. 8 ticków): policz WeightClass/DiverterCommand tak jak w runTicks,
    //    diverter.setCommand(...), diverter.resolve(), advance(plant, diverter), i wypisz numer
    //    ticku, strefę paczki (psm::toString) oraz diverter.actualPosition().
    return 0;
}
