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

extern volatile char c_recv;
extern volatile bool new_char_recv;
// -------- Prototipos de funciones Publicas ---------------

// Inicializacion de Puerto Serie
void SerialPort_Init(uint8_t);

// Inicializacion de Transmisor
void SerialPort_TX_Enable(void);
void SerialPort_TX_Interrupt_Enable(void);
void SerialPort_TX_Interrupt_Disable(void);

// Inicializacion de Receptor
void SerialPort_RX_Enable(void);
void SerialPort_RX_Interrupt_Enable(void);

// Transmision
void SerialPort_Wait_For_TX_Buffer_Free(void); // Pooling - Bloqueante hasta que termine de transmitir.
void SerialPort_Send_Data(char);
void SerialPort_Send_String(char *);
void SerialPort_Send_uint8_t(uint8_t);
void SerialPort_send_int16_t(int val, unsigned int field_length);
void UART_SendString_IT(char *str);

// Recepcion
void SerialPort_Wait_Until_New_Data(void); // Pooling - Bloqueante, puede durar indefinidamente!
char SerialPort_Recive_Data(void);

#endif /* UART_UTILS_H_ */