// Juego.h
#ifndef JUEGO_H
#define JUEGO_H

#include <vector>
#include <memory>
#include <random>
#include "Jugador.h"
#include "Fila.h"

class Juego {
private:
    std::vector<std::unique_ptr<Jugador>> jugadores;
    std::vector<Fila> filas;
    std::vector<Carta> mazo;
    size_t jugadorActual;
    bool ultimaRonda;
    size_t filasActivas;
    std::mt19937 rng;
    int contadorCartas;
    int rondaActual;

public:
    explicit Juego(const std::vector<std::string>& nombresJugadores);
    Juego(const Juego&) = delete;
    Juego& operator=(const Juego&) = delete;

    void jugar();

private:
    void inicializarMazo();
    void inicializarFilas();
    void turno();
    void robarCarta();
    void colocarCartaEnFila(const Carta& carta);
    bool todasLasFilasLlenas() const;
    void tomarFila();
    void reiniciarTurnos();
    bool juegoTerminado() const;
    void calcularPuntuacionFinal() const;
    void calcularPuntuacionRonda();
    int calcularPuntuacion(const Jugador& jugador) const;
    static int calcularPuntosPorColor(int cantidad);
    void mostrarEstadoJuego() const;
    static void mostrarOpcionesFilas(const std::vector<size_t>& filasDisponibles);
    bool preguntarContinuar();
    void prepararNuevaRonda();
};

#endif // JUEGO_H