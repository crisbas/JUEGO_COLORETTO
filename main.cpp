#include <iostream>
#include <vector>
#include <limits>
#include "Juego.h"

int main() {
    std::cout << "Iniciando Coloretto..." << std::endl;

    int numJugadores;
    do {
        std::cout << "¿Cuántos jugadores desean jugar? (mínimo 3, máximo 5): ";
        if (!(std::cin >> numJugadores)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entrada inválida. Por favor, ingresa un número." << std::endl;
            continue;
        }
        if (numJugadores < 3 || numJugadores > 5) {
            std::cout << "Número inválido. Por favor, ingresa un número entre 3 y 5." << std::endl;
        }
    } while (numJugadores < 3 || numJugadores > 5);

    std::vector<std::string> nombresJugadores;
    for (int i = 1; i <= numJugadores; ++i) {
        std::string nombre;
        std::cout << "Ingresa el nombre del Jugador " << i << ": ";
        std::cin >> nombre;
        nombresJugadores.push_back(nombre);
    }

    try {
        Juego juego(nombresJugadores);
        juego.jugar();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
