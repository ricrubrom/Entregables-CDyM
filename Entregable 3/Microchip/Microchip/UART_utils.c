/*
 * UART_utils.c
 *
 * Created: 6/6/2025 14:54:09
 *  Author: mater
 */

#include "UART_utils.h"

// ------ Definiciones de Funciones Públicas -------------------

ISR(USART_RX_vect)
{
  new_char_recv = true; // Indicar que hay un nuevo carácter recibido
}

ISR(USART_TX_vect)
{
  new_char_sent = true; // Indicar que se ha enviado un nuevo carácter
}

void UART_SendString_IT(char *str)
{
  strncpy((char *)tx_buffer, str, BUFFER_SIZE-1);
  tx_buffer[BUFFER_SIZE-1] = '\0';

  tx_index = 0;
  enviando = 1;

  UCSR0B |= (1 << TXCIE0);
  new_char_sent = true;
}
// Inicialización de Puerto Serie

void UART_Init(uint8_t config)
{
  UCSR0B = 0;
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); //8 bits
  UBRR0H = (unsigned char)(config >> 8);
  UBRR0L = (unsigned char)config; //baud rate=9600bps
}

// Inicialización de Transmisor

void UART_TX_Enable(void)
{
  UCSR0B |= (1 << TXEN0);
}

// Inicialización de Receptor

void UART_RX_Enable(void)
{
  UCSR0B |= (1 << RXEN0);
}

void UART_RX_Interrupt_Enable(void)
{
  UCSR0B |= (1 << RXCIE0);
}

// Transmisión
void UART_Send_Data(char data)
{
  UDR0 = data;
}
