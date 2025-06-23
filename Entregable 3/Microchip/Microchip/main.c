/*
 * Microchip.c
 *
 * Created: 2/6/2025 14:40:23
 * Author : Barcala
 */

#include "main.h"
#include "I2C_utils.h"
#include "UART_utils.h"
#include "RTC_utils.h"
#include "Timer1_utils.h"

// RX (Recepción)
char rx_buffer[BUFFER_SIZE];
uint8_t rx_index = 0;
bool nueva_cadena = false;
char c_recv;
volatile bool new_char_recv = false;

// TX (Transmisión)
char tx_buffer[BUFFER_SIZE];
uint8_t tx_index = 0;
char c_sent;
uint8_t enviando = 0;
volatile bool new_char_sent = false;

// Flags y estados generales
bool alarm = false;
bool powered = false;
bool alarm_times = 0;
volatile bool time_flag = false;

// RTC
RTC_t time;
RTC_t alarm_time;

RTC_Field current_read = DAY;

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
	else if (rx_index < sizeof(rx_buffer) - 1) // Evitar overflow del buffer
	{
		rx_buffer[rx_index++] = c_recv; // Agregar al buffer
	}
}

RTC_t manage_input(RTC_Field start)
{
	RTC_t aux;
	for (current_read = start; current_read <= SEC; current_read++)
	{
		while (!nueva_cadena)
		{
			if (new_char_recv)
			{
				c_recv = UDR0;
				new_char_recv = false;
				save_char();
			}
		}
		nueva_cadena = false;
		switch (current_read)
		{
		case DAY:
			aux.day = atoi(rx_buffer);
			break;
		case MONTH:
			aux.month = atoi(rx_buffer);
			break;
		case YEAR:
			aux.year = (atoi(rx_buffer) % 100);
			break;
		case HOUR:
			aux.hours = atoi(rx_buffer);
			break;
		case MIN:
			aux.minutes = atoi(rx_buffer);
			break;
		case SEC:
			aux.seconds = atoi(rx_buffer);
			break;
		default:
			break;
		}
	}
	return aux; // Devolver la estructura RTC con los valores leídos
}

void manage_new_string()
{
	if (strcmp(rx_buffer, "on") == 0 || strcmp(rx_buffer, "ON") == 0)
	{
		powered = true;
	}
	else if (strcmp(rx_buffer, "off") == 0 || strcmp(rx_buffer, "OFF") == 0)
	{
		powered = false;
		UART_SendString_IT("Comando 'OFF' recibido.");
	}
	else if (strcmp(rx_buffer, "set_alarm") == 0 || strcmp(rx_buffer, "SET_ALARM") == 0)
	{
		alarm_time = manage_input(HOUR);
	}
	else if (strcmp(rx_buffer, "set_time") == 0 || strcmp(rx_buffer, "SET_TIME") == 0)
	{
		RTC_SetTime(manage_input(DAY));
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

bool compare()
{
	return (time.hours == alarm_time.hours) &&
				 (time.minutes == alarm_time.minutes) &&
				 (time.seconds == alarm_time.seconds);
}

void manage_alarm()
{
	alarm_times++;
	UART_SendString_IT("ALARMA");
	if (alarm_times >= 5)
	{
		alarm = false;
		alarm_times = 0;
		time = RTC_GetTime(); // Actualizar la hora actual
	}
}

void print_time()
{
	char *str[BUFFER_SIZE];
	sprintf((char *)str, "FECHA: %02d/%02d/%02d HORA:%02d:%02d:%02d", time.day, time.month, time.year, time.hours, time.minutes, time.seconds);
	UART_SendString_IT(str);
}

void init()
{
	RTC_Init();
	UART_Init(BR9600);					// Configurar UART a 9600bps, 8 bits de datos, 1 bit de parada
	UART_TX_Enable();						// Habilitar transmisor
	UART_RX_Enable();						// Habilitar receptor
	UART_RX_Interrupt_Enable(); // Habilitar interrupción de recepción
	Timer1_init();
	UART_SendString_IT("Bienvenidos\r\nComandos:\r\n- on / ON\r\n- off / OFF \r\n- set_time / SET_TIME\r\n - Dia \r\n - Mes \r\n - Anio \r\n - Hora \r\n - Min \r\n - Seg \r\n- set_alarm / SET_ALARM\r\n - Hora \r\n - Min \r\n - Seg \r\n"); // Envío el mensaje de Bienvenida
	sei();																																																																																																																		 // habilitar interrupciones globales
}

int main(void)
{
	init(); // Inicializar el sistema
	while (1)
	{
		if (new_char_recv)
		{
			c_recv = UDR0;
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
		if (time_flag)
		{
			time_flag = false;
			time = RTC_GetTime();
			if (alarm)
				manage_alarm(); // Manejar la alarma
			else if (powered)
				print_time(); // Imprimir la hora actual
			if (!alarm && compare(time, alarm_time))
			{
				alarm = true;
			}
		}
	}
}