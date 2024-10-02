// Carta.cpp
#include "Carta.h"

Carta::Carta(std::string c, TipoCarta t, bool comodin, bool finalizacion) noexcept
    : color(std::move(c)), tipo(t), esComodin(comodin), esFinalizacion(finalizacion) {}

bool Carta::esCartaEspecial() const noexcept {
    return tipo == TipoCarta::Especial || esComodin;
}