// Jugador.h
#ifndef JUGADOR_H
#define JUGADOR_H

#include <string>
#include <vector>
#include "Carta.h"

class Jugador {
public:
    std::string nombre;
    std::vector<Carta> mano;
    bool haTomadoFila;

    explicit Jugador(std::string n) noexcept;
};

#endif // JUGADOR_H