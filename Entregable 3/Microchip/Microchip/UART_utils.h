/*
 * UART_utils.h
 *
 * Created: 6/6/2025 14:53:55
 *  Author: mater
 */

#ifndef UART_UTILS_H_
#define UART_UTILS_H_

// ------------------- Includes ----------------------------

// Archivo de cabecera del Microcontrolador
#include <avr/io.h>

// Interrupciones del Microcontrolador
#include <avr/interrupt.h>

// Tipo de dato booleano
#include "main.h"

// extern volatile char c_recv;
extern volatile bool new_char_recv;
extern volatile uint8_t tx_index;
extern volatile bool enviado;
extern volatile char tx_buffer[BUFFER_SIZE];
// -------- Prototipos de funciones Publicas ---------------

// Inicializacion de Puerto Serie
void UART_Init(uint8_t);

// Inicializacion de Transmisor
void UART_TX_Enable(void);
void UART_TX_Interrupt_Enable(void);
void UART_TX_Interrupt_Disable(void);

// Inicializacion de Receptor
void UART_RX_Enable(void);
void UART_RX_Interrupt_Enable(void);

// Transmision
void UART_Wait_For_TX_Buffer_Free(void); // Pooling - Bloqueante hasta que termine de transmitir.
void UART_Send_Data(char);
void UART_Send_String(char *);
void UART_Send_uint8_t(uint8_t);
void UART_send_int16_t(int val, unsigned int field_length);
void UART_SendString_IT(char *str);

// Recepcion
void UART_Wait_Until_New_Data(void); // Pooling - Bloqueante, puede durar indefinidamente!
char UART_Recive_Data(void);

#endif /* UART_UTILS_H_ */