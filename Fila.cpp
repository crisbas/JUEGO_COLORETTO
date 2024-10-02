// Fila.cpp
#include "Fila.h"

Fila::Fila() noexcept : activa(true) {}

bool Fila::agregarCarta(const Carta& carta) noexcept {
    if (activa && cartas.size() < 3) {
        cartas.push_back(carta);
        return true;
    }
    return false;
}
