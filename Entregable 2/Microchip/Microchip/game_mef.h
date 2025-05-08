/**
 * @file game_mef.h
 * @brief Declaraciones de funciones relacionadas con la máquina de estados del juego.
 *
 * Este archivo contiene las declaraciones de todas las funciones que manejan
 * los diferentes estados del juego de ingreso de contraseñas.
 */

#include "game_utils.h"

#ifndef GAME_MEF_H
#define GAME_MEF_H

/// Estado actual del juego
extern volatile GameState gameState;

/// Tick general (para mostrar la contraseña, ganar o perder)
extern volatile uint8_t tick;

/// Segundos contados mientras se muestra la contraseña o tras finalizar el juego
extern volatile uint8_t second;

/// Tick durante el juego activo
extern volatile uint8_t gameTick;

/// Segundos contados durante el juego activo
extern volatile uint8_t gameSecond;

/// Bandera de temporización, cambia en cada interrupción
extern volatile uint8_t temporization_flag;

/**
 * @brief Ejecuta un paso de la máquina de estados del juego.
 *
 * Según el estado actual del juego, delega el control a la función correspondiente.
 * Esta función debe ser llamada periódicamente, idealmente desde el lazo principal.
 */
void game_mef(void);

/**
 * @brief Maneja el estado de inicio del juego.
 *
 * Espera que el usuario presione '*' para iniciar una nueva partida. Selecciona
 * una contraseña aleatoria del diccionario y la muestra por pantalla.
 */
void handle_start_state(void);

/**
 * @brief Maneja el estado de visualización de la contraseña.
 *
 * Espera un tiempo determinado para mostrar la contraseña y luego
 * inicializa las variables del juego y cambia al estado de juego.
 */
void handle_show_password_state(void);

/**
 * @brief Maneja el estado principal del juego.
 *
 * Escanea el teclado para recibir la entrada del usuario. Si se presiona '#',
 * se compara la entrada con el carácter correspondiente de la contraseña.
 * Si es correcto se cambia al estado HIT, si es incorrecto, al estado MISS.
 */
void handle_game_state(void);

/**
 * @brief Maneja el estado de acierto (HIT).
 *
 * Actualiza la pantalla y las variables para pasar al siguiente carácter
 * de la contraseña. Si se completa la contraseña, cambia al estado WIN.
 */
void handle_hit_state(void);

/**
 * @brief Maneja el estado de error (MISS).
 *
 * Resta una vida, actualiza el display y verifica si se terminó el juego.
 * Si no quedan vidas, cambia al estado LOSE.
 */
void handle_miss_state(void);

/**
 * @brief Maneja el estado de victoria (WIN).
 *
 * Espera unos segundos y vuelve al estado de inicio.
 */
void handle_win_state(void);

/**
 * @brief Maneja el estado de derrota (LOSE).
 *
 * Espera unos segundos y vuelve al estado de inicio.
 */
void handle_lose_state(void);

#endif
