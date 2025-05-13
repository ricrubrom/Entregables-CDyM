/**
 * @file game_utils.h
 * @brief Declaraciones y variables globales para el manejo de temporización en el juego.
 *
 * Contiene variables de temporización
 * y la declaración de la función de inicialización del temporizador.
 */
#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef TIMER_H
#define TIMER_H

/// Tick general (para mostrar la contraseña, ganar o perder)
extern volatile uint8_t tick;

/// Segundos contados mientras se muestra la contraseña o tras finalizar el juego
extern volatile uint8_t second;

/// Tick durante el juego activo
extern volatile uint8_t gameTick;

/// Segundos contados durante el juego activo
extern volatile uint8_t gameSecond;


/**
 * @brief Inicializa el temporizador Timer0 en modo CTC.
 *
 * Configura el Timer0 con un prescaler de 1024 y un valor de comparación de 155,
 * lo que genera interrupciones aproximadamente cada 10 ms.
 */
void timer_init(void);

#endif
