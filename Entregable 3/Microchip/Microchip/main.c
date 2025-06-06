/*
 * Microchip.c
 *
 * Created: 2/6/2025 14:40:23
 * Author : Barcala
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "I2C_utils.h"
#include "UART_utils.h"
#include "RTC_utils.h"
#include "main.h"
#include <string.h>

#define BR9600 (0x67)
#define BUFFER_SIZE 64

volatile char rx_buffer[BUFFER_SIZE];
volatile uint8_t rx_index = 0;
volatile bool nueva_cadena = false;
volatile char c_recv;
volatile bool new_char_recv = false;
volatile char tx_buffer[BUFFER_SIZE];
volatile uint8_t tx_index = 0;
volatile uint8_t enviando = 0;
volatile char c_sent;
volatile bool new_char_sent = false;

void save_char()
{
	if (c_recv == '\b' || c_recv == 0x7F) // Si se recibe un carácter de retroceso
	{
		if (rx_index > 0) // Evitar que el índice sea negativo
		{
			rx_index--; // Retroceder el índice
		}
	}
	else if (c_recv == '\r') // Si se recibe un retorno de carro
	{
		rx_buffer[rx_index] = '\0'; // Terminar la cadena
		rx_index = 0;								// Reiniciar el índice
		nueva_cadena = true;				// Indicar que hay una nueva cadena
	}
	else
	{
		if (rx_index < sizeof(rx_buffer) - 1) // Evitar overflow del buffer
		{
			rx_buffer[rx_index++] = c_recv; // Agregar al buffer
		}
	}
}
void manage_new_string()
{
	if (strcmp(rx_buffer, "on") == 0 || strcmp(rx_buffer, "ON") == 0)
	{
		// Aquí se puede agregar la lógica para manejar el comando "on"
		UART_SendString_IT("Comando 'ON' recibido.");
	}
	else if (strcmp(rx_buffer, "off") == 0 || strcmp(rx_buffer, "OFF") == 0)
	{
		// Aquí se puede agregar la lógica para manejar el comando "off"
		UART_SendString_IT("Comando 'OFF' recibido.");
	}
	else if (strcmp(rx_buffer, "set_alarm") == 0 || strcmp(rx_buffer, "SET_ALARM") == 0)
	{
		// Aquí se puede agregar la lógica para manejar el comando "set_alarm"
		UART_SendString_IT("Comando 'set_alarm' recibido.");
	}
	else if (strcmp(rx_buffer, "set_time") == 0 || strcmp(rx_buffer, "SET_TIME") == 0)
	{
		// Aquí se puede agregar la lógica para manejar el comando "set_time"
		UART_SendString_IT("Comando 'set_time' recibido.");
	}
	else
	{
		UART_SendString_IT("Comando no reconocido.");
	}
}

void manage_tx_buffer()
{
	if (enviando == 1)
	{
		c_sent = tx_buffer[tx_index];
		if (c_sent != '\0')
		{
			SerialPort_Send_Data(c_sent);
			tx_index++;
		}
		else
		{
			SerialPort_Send_Data('\r');
			enviando = 2;
		}
	}
	else if (enviando == 2)
	{
		SerialPort_Send_Data('\n');
		enviando = 0;
		tx_index = 0;							// Reiniciar el índice de transmisión
		UCSR0B &= ~(1 << TXCIE0); // Deshabilita interrupción TX
	}
}

void UART_SendString_IT(char *str)
{
	strncpy((char *)tx_buffer, str, BUFFER_SIZE - 3);
	tx_buffer[BUFFER_SIZE - 3] = '\0';

	tx_index = 0;
	enviando = 1;

	UCSR0B |= (1 << TXCIE0);
	SerialPort_Send_Data(tx_buffer[tx_index]);
}

ISR(USART_TX_vect)
{
	new_char_sent = true; // Indicar que se ha enviado un nuevo carácter
}

int main(void)
{
	// i2c_init();
	// i2c_start();
	// i2c_write(0xD0);
	// i2c_write(0);
	// i2c_write(10);
	// i2c_write(11);
	// i2c_write(12);
	// i2c_stop();
	SerialPort_Init(BR9600);																					// Configurar UART a 9600bps, 8 bits de datos, 1 bit de parada
	SerialPort_TX_Enable();																						// Habilitar transmisor
	SerialPort_RX_Enable();																						// Habilitar receptor
	SerialPort_RX_Interrupt_Enable();																	// Habilitar interrupción de recepción
	SerialPort_Send_String("Bienvenidos a mi canal de youtube.\r\n"); // Envío el mensaje de Bienvenida
	sei();																														// habilitar interrupciones globales

	while (1)
	{
		if (new_char_recv)
		{
			new_char_recv = false; // Reiniciar el indicador
			save_char();					 // Guardar el carácter recibido
		}
		if (nueva_cadena)
		{
			nueva_cadena = false; // Reiniciar el indicador
			manage_new_string();	// Manejar la nueva cadena recibida
		}
		if (new_char_sent)
		{
			new_char_sent = false; // Reiniciar el indicador
			manage_tx_buffer();		 // Manejar el buffer de transmisión
		}
	}
}

// #include <avr/io.h>
// #include <avr/interrupt.h>
// #include <stdbool.h>
// #include <string.h>

// #define F_CPU 16000000UL
// #define BAUD 9600
// #define UBRR_VALUE ((F_CPU / (16UL * BAUD)) - 1)

// #define BUFFER_SIZE 64

// // Buffers y estados
// volatile char rx_buffer[BUFFER_SIZE];
// volatile uint8_t rx_index = 0;
// volatile bool nueva_cadena = false;

// volatile char tx_buffer[BUFFER_SIZE];
// volatile uint8_t tx_index = 0;
// volatile uint8_t enviando = 0;

// void UART_Init(void) {
// 	UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
// 	UBRR0L = (uint8_t)(UBRR_VALUE);

// 	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
// 	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
// }

// void UART_SendChar(char c) {
// 	while (!(UCSR0A & (1 << UDRE0)));
// 	UDR0 = c;
// }

// void SerialPort_Send_String(char *str) {
// 	while (*str) {
// 		UART_SendChar(*str++);
// 	}
// }

// void UART_SendString_IT(char *str) {
// 	strncpy((char *)tx_buffer, str, BUFFER_SIZE - 3);
// 	tx_buffer[BUFFER_SIZE - 3] = '\0';

// 	tx_index = 0;
// 	enviando = 1;

// 	UCSR0B |= (1 << TXCIE0);
// 	UART_SendChar(tx_buffer[tx_index++]);
// }

// // ISR de recepci�n
// ISR(USART_RX_vect) {
// 	char c = UDR0;
// 	if (rx_index < BUFFER_SIZE - 1) {
// 		if (c == '\r') {
// 			rx_buffer[rx_index] = '\0';
// 			rx_index = 0;
// 			nueva_cadena = true;
// 			} else {
// 			rx_buffer[rx_index++] = c;
// 		}
// 		} else {
// 		rx_index = 0; // Overflow de buffer
// 	}
// }

// // ISR de transmisi�n completa
// ISR(USART_TX_vect) {
// 	if (enviando == 1) {
// 		char c = tx_buffer[tx_index];
// 		if (c != '\0') {
// 			UART_SendChar(c);
// 			tx_index++;
// 			} else {
// 			UART_SendChar('\r');
// 			enviando = 2;
// 		}
// 		} else if (enviando == 2) {
// 		UART_SendChar('\n');
// 		enviando = 0;
// 		UCSR0B &= ~(1 << TXCIE0); // Deshabilita interrupci�n TX
// 	}
// }

// int main(void) {
// 	UART_Init();
// 	sei();

// 	while (1) {
// 		if (nueva_cadena) {
// 			nueva_cadena = false;
// 			UART_SendString_IT((char *)rx_buffer);
// 		}
// 	}
// }