/*
 * UART_utils.h
 *
 * Created: 6/6/2025 14:53:55
 *  Author: mater
 */

#ifndef UART_UTILS_H_
#define UART_UTILS_H_

// ------------------- Includes ----------------------------

// Tipo de dato booleano
#include "main.h"

extern uint8_t tx_index;
extern bool enviado;
extern uint8_t enviando;
extern char tx_buffer[BUFFER_SIZE];
extern volatile bool new_char_recv;
extern volatile bool new_char_sent;
extern volatile char c_recv;
// -------- Prototipos de funciones Publicas ---------------

// Inicializacion de Puerto Serie
void UART_Init(uint8_t);

// Inicializacion de Transmisor
void UART_TX_Enable(void);

// Inicializacion de Receptor
void UART_RX_Enable(void);
void UART_RX_Interrupt_Enable(void);

// Transmision
void UART_Send_Data(char);
void UART_SendString_IT(char *str);

#endif /* UART_UTILS_H_ */