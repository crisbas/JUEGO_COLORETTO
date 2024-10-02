// Juego.cpp
#include "Juego.h"
#include <iostream>
#include <algorithm>
#include <map>
#include <limits>
#include "Colors.h"

Juego::Juego(const std::vector<std::string>& nombresJugadores)
    : jugadorActual(0), ultimaRonda(false), filasActivas(nombresJugadores.size()), rng(std::random_device{}()) {
    for (const auto& nombre : nombresJugadores) {
        jugadores.push_back(std::make_unique<Jugador>(nombre));
    }
    inicializarMazo();
    inicializarFilas();
}

void Juego::inicializarMazo() {
    std::vector<std::string> colores = {"Rojo", "Azul", "Amarillo", "Verde", "Naranja", "Morado", "Café"};

    for (const auto& color : colores) {
        for (int i = 0; i < 9; ++i) {
            mazo.emplace_back(color, TipoCarta::Color);
        }
    }

    for (int i = 0; i < 10; ++i) {
        mazo.emplace_back("Especial", TipoCarta::Especial);
    }

    mazo.emplace_back("Comodín", TipoCarta::Comodin, true);
    mazo.emplace_back("Comodín dorado", TipoCarta::Comodin, true);

    std::vector<Carta> nuevoMazo;
    for (size_t i = 0; i < mazo.size(); ++i) {
        if ((i > 0) && (i % 16 == 0)) {
            nuevoMazo.emplace_back("Fin del juego", TipoCarta::Finalizacion, false, true);
        }
        nuevoMazo.push_back(mazo[i]);
    }
    mazo = std::move(nuevoMazo);

    std::shuffle(mazo.begin(), mazo.end(), rng);
}

void Juego::inicializarFilas() {
    filas.resize(jugadores.size());
}

void Juego::jugar() {
    while (!juegoTerminado()) {
        turno();
        do {
            jugadorActual = (jugadorActual + 1) % jugadores.size();
        } while (jugadores[jugadorActual]->haTomadoFila && filasActivas > 0);
    }
    calcularPuntuacionFinal();
}

void Juego::turno() {
    Jugador& jugadorActivo = *jugadores[jugadorActual];
    std::cout << "Turno de " << jugadorActivo.nombre << std::endl;
    mostrarEstadoJuego();

    if (jugadorActivo.haTomadoFila) {
        std::cout << jugadorActivo.nombre << " ya ha tomado una fila. Pasando al siguiente jugador." << std::endl;
        return;
    }

    if (todasLasFilasLlenas()) {
        std::cout << "Todas las filas activas están llenas. Debes tomar una fila." << std::endl;
        tomarFila();
        return;
    }

    char eleccion;
    do {
        std::cout << "¿Deseas robar una carta (R) o tomar una fila (F)? ";
        std::cin >> eleccion;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } while (eleccion != 'R' && eleccion != 'r' && eleccion != 'F' && eleccion != 'f');

    if (eleccion == 'R' || eleccion == 'r') {
        robarCarta();
    } else if (eleccion == 'F' || eleccion == 'f') {
        tomarFila();
    }
}

void Juego::robarCarta() {
    if (mazo.empty()) {
        std::cout << "El mazo está vacío. Fin del juego." << std::endl;
        ultimaRonda = true;
        return;
    }

    Carta carta = mazo.back();
    mazo.pop_back();

    if (carta.esFinalizacion) {
        std::cout << "¡Se ha robado la carta de finalización! Comienza la última ronda." << std::endl;
        ultimaRonda = true;
        return;
    }

    if (carta.tipo == TipoCarta::Especial) {
        std::cout << "Has robado una carta +2. Ganas 2 puntos adicionales." << std::endl;
        jugadores[jugadorActual]->mano.push_back(carta);
        return;
    }

    if (carta.esComodin) {
        std::cout << "Has robado un comodín." << std::endl;
        jugadores[jugadorActual]->mano.push_back(carta);
        return;
    }

    std::string colorTexto;
    if (carta.color == "Rojo") colorTexto = GameColors::ROJO;
    else if (carta.color == "Azul") colorTexto = GameColors::AZUL;
    else if (carta.color == "Amarillo") colorTexto = GameColors::AMARILLO;
    else if (carta.color == "Verde") colorTexto = GameColors::VERDE;
    else if (carta.color == "Naranja") colorTexto = GameColors::NARANJA;
    else if (carta.color == "Morado") colorTexto = GameColors::MORADO;
    else if (carta.color == "Café") colorTexto = GameColors::MARRON;
    else colorTexto = GameColors::RESET;

    GameColors::printColored("Has robado una carta " + carta.color, colorTexto);

    if (todasLasFilasLlenas()) {
        std::cout << "Todas las filas activas están llenas. Debes tomar una fila." << std::endl;
        tomarFila();
        return;
    }

    colocarCartaEnFila(carta);
}

void Juego::colocarCartaEnFila(const Carta& carta) {
    std::cout << "¿En qué fila activa quieres colocarla? ";

    std::vector<size_t> filasDisponibles;
    for (size_t i = 0; i < filas.size(); ++i) {
        if (filas[i].activa && filas[i].cartas.size() < 3) {
            filasDisponibles.push_back(i);
        }
    }

    mostrarOpcionesFilas(filasDisponibles);

    size_t filaElegida;
    while (true) {
        if (!(std::cin >> filaElegida)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entrada inválida. Por favor, ingresa un número: ";
            continue;
        }
        filaElegida--;
        if (std::find(filasDisponibles.begin(), filasDisponibles.end(), filaElegida) != filasDisponibles.end()) {
            break;
        }
        std::cout << "Fila inválida. Por favor, elige una fila disponible: ";
    }

    filas[filaElegida].agregarCarta(carta);
    std::cout << "Carta colocada en la fila " << (filaElegida + 1) << std::endl;
}

bool Juego::todasLasFilasLlenas() const {
    return std::all_of(filas.begin(), filas.end(), [](const Fila& f) { 
        return !f.activa || f.cartas.size() >= 3; 
    });
}

void Juego::tomarFila() {
    std::cout << "¿Qué fila activa quieres tomar? ";

    std::vector<size_t> filasActivasIndices;
    for (size_t i = 0; i < filas.size(); ++i) {
        if (filas[i].activa) {
            filasActivasIndices.push_back(i);
        }
    }

    mostrarOpcionesFilas(filasActivasIndices);

    size_t filaElegida;
    while (true) {
        if (!(std::cin >> filaElegida)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entrada inválida. Por favor, ingresa un número: ";
            continue;
        }
        filaElegida--;
        if (std::find(filasActivasIndices.begin(), filasActivasIndices.end(), filaElegida) != filasActivasIndices.end()) {
            break;
        }
        std::cout << "Fila inválida. Por favor, elige una fila activa: ";
    }

    for (const auto& carta : filas[filaElegida].cartas) {
        jugadores[jugadorActual]->mano.push_back(carta);
    }
    filas[filaElegida].cartas.clear();
    filas[filaElegida].activa = false;
    jugadores[jugadorActual]->haTomadoFila = true;
    filasActivas--;
    std::cout << jugadores[jugadorActual]->nombre << " ha tomado la fila " << (filaElegida + 1) << std::endl;

    if (filasActivas == 0) {
        reiniciarTurnos();
    }
}
void Juego::reiniciarTurnos() {
    for (auto& jugador : jugadores) {
        jugador->haTomadoFila = false;
    }
    for (auto& fila : filas) {
        fila.activa = true;
    }
    filasActivas = jugadores.size();
    std::cout << "Todos los jugadores han tomado una fila. Se reinician los turnos y las filas." << std::endl;
}

bool Juego::juegoTerminado() const {
    return (ultimaRonda && filasActivas == filas.size()) || mazo.empty();
}

void Juego::calcularPuntuacionFinal() const {
    std::vector<std::pair<std::string, int>> puntuaciones;
    for (const auto& jugador : jugadores) {
        int puntuacion = calcularPuntuacion(*jugador);
        puntuaciones.emplace_back(jugador->nombre, puntuacion);
    }

    std::sort(puntuaciones.begin(), puntuaciones.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });

    std::cout << "\nPuntuaciones finales:\n";
    for (const auto& [nombre, puntos] : puntuaciones) {
        std::cout << nombre << " - Puntuación: " << puntos << std::endl;
    }

    std::cout << "\n¡El ganador es " << puntuaciones[0].first << " con " << puntuaciones[0].second << " puntos!" << std::endl;
}

int Juego::calcularPuntuacion(const Jugador& jugador) const {
    std::map<std::string, int> conteoColores;
    int comodines = 0;

    for (const auto& carta : jugador.mano) {
        if (carta.esComodin) {
            comodines++;
        } else if (carta.tipo == TipoCarta::Especial) {
            conteoColores["+2"] += 2;
        } else {
            conteoColores[carta.color]++;
        }
    }

    std::vector<std::pair<std::string, int>> coloresOrdenados(conteoColores.begin(), conteoColores.end());
    std::sort(coloresOrdenados.begin(), coloresOrdenados.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });

    int puntuacion = 0;
    for (const auto& [color, cantidad] : coloresOrdenados) {
        puntuacion += calcularPuntosPorColor(cantidad);
    }

    puntuacion += comodines * 3;

    return puntuacion;
}

int Juego::calcularPuntosPorColor(int cantidad) {
    static const std::vector<int> puntos = {0, 1, 3, 6, 10, 15, 21};
    return puntos[std::min(cantidad, 6)];
}

void Juego::mostrarEstadoJuego() const {
    std::cout << "\nEstado del juego:" << std::endl;
    for (const auto& jugador : jugadores) {
        std::cout << jugador->nombre << " tiene " << jugador->mano.size() << " cartas." << std::endl;
    }

    std::cout << "\nFilas activas:" << std::endl;
    for (size_t i = 0; i < filas.size(); ++i) {
        if (filas[i].activa) {
            std::cout << "Fila " << (i + 1) << ": ";
            for (const auto& carta : filas[i].cartas) {
                std::cout << carta.color << " ";
            }
            std::cout << std::endl;
        }
    }

    std::cout << "\nCartas restantes en el mazo: " << mazo.size() << std::endl;
}

void Juego::mostrarOpcionesFilas(const std::vector<size_t>& filasDisponibles) {
    for (size_t i : filasDisponibles) {
        std::cout << (i + 1) << " ";
    }
    std::cout << ": ";
}

