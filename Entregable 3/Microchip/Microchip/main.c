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
volatile bool alarm = false;
volatile bool powered = false;

void manage_alarm()
{
	// Limpiar bandera A1F para futuras alarmas usando tus funciones I2C
	I2C_start();
	I2C_write(RTC_WriteMode);				 // Dirección de escritura RTC
	I2C_write(RTC_StatusRegAddress); // Registro de estado
	I2C_stop();

	I2C_start();
	I2C_write(RTC_ReadMode); // Dirección de lectura RTC
	uint8_t status = I2C_read(true);
	I2C_stop();

	status &= ~0x01; // Limpiar A1F

	I2C_start();
	I2C_write(RTC_WriteMode);
	I2C_write(RTC_StatusRegAddress);
	I2C_write(status);
	I2C_stop();
}

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
		powered = true;
		UART_SendString_IT("Comando 'ON' recibido.");
	}
	else if (strcmp(rx_buffer, "off") == 0 || strcmp(rx_buffer, "OFF") == 0)
	{
		powered = false;
		UART_SendString_IT("Comando 'OFF' recibido.");
	}
	else if (strcmp(rx_buffer, "set_alarm") == 0 || strcmp(rx_buffer, "SET_ALARM") == 0)
	{
		RTC_t rtc;
		RTC_SetAlarm(rtc);
		UART_SendString_IT("Comando 'set_alarm' recibido.");
	}
	else if (strcmp(rx_buffer, "set_time") == 0 || strcmp(rx_buffer, "SET_TIME") == 0)
	{
		// Aquí se puede agregar la lógica para manejar el comando "set_time"
		RTC_SetTime(rtc);
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
			UART_Send_Data(c_sent);
			tx_index++;
		}
		else
		{
			UART_Send_Data('\r');
			enviando = 2;
		}
	}
	else if (enviando == 2)
	{
		UART_Send_Data('\n');
		enviando = 0;
		tx_index = 0;							// Reiniciar el índice de transmisión
		UCSR0B &= ~(1 << TXCIE0); // Deshabilita interrupción TX
	}
}

void INT0_init(void)
{
	EIMSK |= (1 << INT0);	 // Habilita interrupción externa INT0
	EICRA |= (1 << ISC01); // Interrupción en flanco descendente (INT/SQW se va a LOW)
}

int main(void)
{
	INT0_init();
	RCT_Init();
	UART_Init(BR9600);					// Configurar UART a 9600bps, 8 bits de datos, 1 bit de parada
	UART_TX_Enable();						// Habilitar transmisor
	UART_RX_Enable();						// Habilitar receptor
	UART_RX_Interrupt_Enable(); // Habilitar interrupción de recepción
	UART_TX_Interrupt_Enable();
	// No la habilitamos la interrupcion del tx????
	UART_Send_String("Bienvenidos a mi canal de youtube.\r\n"); // Envío el mensaje de Bienvenida
	sei();																											// habilitar interrupciones globales

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
		if (alarm)
		{
			alarm = false;
			manage_alarm();
		}
	}
}