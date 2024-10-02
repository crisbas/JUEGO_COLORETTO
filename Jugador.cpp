// Jugador.cpp
#include "Jugador.h"

Jugador::Jugador(std::string n) noexcept : nombre(std::move(n)), haTomadoFila(false) {}
