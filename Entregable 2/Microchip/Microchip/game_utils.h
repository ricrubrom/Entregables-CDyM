/**
 * @file game_utils.h
 * @brief Declaraciones y variables globales para el manejo de temporización en el juego.
 *
 * Contiene la definición del enum de estados del juego, variables de temporización
 * y la declaración de la función de inicialización del temporizador.
 */
#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef GAME_UTILS_H
#define GAME_UTILS_H

/**
 * @brief Inicializa el temporizador Timer0 en modo CTC.
 *
 * Configura el Timer0 con un prescaler de 1024 y un valor de comparación de 155,
 * lo que genera interrupciones aproximadamente cada 10 ms.
 */
void timer_init(void);

/**
 * @brief Estados posibles del juego.
 */
typedef enum
{
    START,         ///< Esperando inicio (*)
    SHOW_PASSWORD, ///< Mostrando la contraseña
    GAME,          ///< Juego en progreso
    HIT,           ///< Acierto en un carácter
    MISS,          ///< Error en un carácter
    WIN,           ///< El jugador ganó
    LOSE           ///< El jugador perdió
} GameState;

#endif
