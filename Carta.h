// Carta.h
#ifndef CARTA_H
#define CARTA_H

#include <string>

enum class TipoCarta { Color, Especial, Comodin, Finalizacion };

class Carta {
public:
    std::string color;
    TipoCarta tipo;
    bool esComodin;
    bool esFinalizacion;

    Carta(std::string c, TipoCarta t = TipoCarta::Color, bool comodin = false, bool finalizacion = false) noexcept;
    bool esCartaEspecial() const noexcept;
};

#endif // CARTA_H