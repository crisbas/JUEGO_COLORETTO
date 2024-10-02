// Fila.h
#ifndef FILA_H
#define FILA_H

#include <vector>
#include "Carta.h"

class Fila {
public:
    std::vector<Carta> cartas;
    bool activa;

    Fila() noexcept;
    bool agregarCarta(const Carta& carta) noexcept;
};

#endif // FILA_H
