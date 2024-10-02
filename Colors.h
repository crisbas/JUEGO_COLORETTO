// Colors.h
#ifndef COLORS_H
#define COLORS_H

#include <string>

namespace GameColors {
    const std::string ROJO = "\033[31m";
    const std::string AZUL = "\033[34m";
    const std::string AMARILLO = "\033[33m";
    const std::string VERDE = "\033[32m";
    const std::string NARANJA = "\033[38;5;208m";
    const std::string MORADO = "\033[35m";
    const std::string MARRON = "\033[38;5;94m";
    const std::string RESET = "\033[0m";

    void printColored(const std::string& text, const std::string& color);
}

#endif // COLORS_H