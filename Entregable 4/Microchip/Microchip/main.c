#include "main.h"
#include "UART_utils.h"
#include "Timer1_utils.h"
#include "Timer0_utils.h"
#include "ADC_utils.h"

// RX (Recepción)
char rx_buffer[BUFFER_SIZE];
uint8_t rx_index = 0;
bool nueva_cadena = false;
char c_recv;
volatile bool new_char_recv = false;

uint8_t r = 255, g = 255, b = 255;
volatile bool time_flag = false;
uint8_t pwm_rojo = 0;

// TX (Transmisión)
char tx_buffer[BUFFER_SIZE];
uint8_t tx_index = 0;
char c_sent;
uint8_t enviando = 0;
volatile bool new_char_sent = false;

uint8_t apply_brightness(uint8_t color_component, uint8_t adc_value)
{
	return 255 - ((255 - color_component) * adc_value) / 255;
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
	else if (rx_index < sizeof(rx_buffer) - 1) // Evitar overflow del buffer
	{
		rx_buffer[rx_index++] = c_recv; // Agregar al buffer
	}
}

void manage_input()
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
	return;
}

void manage_new_string()
{
	if (strcmp(rx_buffer, "1") == 0)
	{
		UART_SendString_IT("Color actualizado.");
		r = 0;
		g = 255;
		b = 255;
	}
	else if (strcmp(rx_buffer, "2") == 0)
	{
		UART_SendString_IT("Color actualizado.");
		r = 255;
		g = 0;
		b = 255;
	}
	else if (strcmp(rx_buffer, "3") == 0)
	{
		UART_SendString_IT("Color actualizado.");
		r = 255;
		g = 255;
		b = 0;
	}
	else if (strcmp(rx_buffer, "4") == 0)
	{
		UART_SendString_IT("Color actualizado.");
		r = 255;
		g = 0;
		b = 0;
	}
	else if (strcmp(rx_buffer, "5") == 0)
	{
		UART_SendString_IT("Color actualizado.");
		r = 0;
		g = 0;
		b = 255;
	}
	else if (strcmp(rx_buffer, "6") == 0)
	{
		UART_SendString_IT("Color actualizado.");
		r = 0;
		g = 255;
		b = 0;
	}
	else if (strcmp(rx_buffer, "7") == 0)
	{
		UART_SendString_IT("Color actualizado.");
		r = 0;
		g = 0;
		b = 0;
	}
	else if (strcmp(rx_buffer, "8") == 0)
	{
		UART_SendString_IT("Color actualizado.");
		r = 255;
		g = 255;
		b = 255;
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

void init()
{
	DDRB = 0b11111111;
	UART_Init(BR9600);					// Configurar UART a 9600bps, 8 bits de datos, 1 bit de parada
	UART_TX_Enable();						// Habilitar transmisor
	UART_RX_Enable();						// Habilitar receptor
	UART_RX_Interrupt_Enable(); // Habilitar interrupción de recepción
	ADC_init();
	Timer1_init();
	timer0_init();
	UART_SendString_IT("Bienvenidos\r\nColores:\r\n- red\r\n- green\r\n- blue\r\n- cian\r\n- yellow\r\n- magenta\r\n- white\r\n- black\r\n"); // Envío el mensaje de Bienvenida
	sei();																																																																		// habilitar interrupciones globales
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

		uint8_t adc = ADC_get_value(); // brillo entre 0–255

		OCR1A = apply_brightness(b, adc);		 // azul
		OCR1B = apply_brightness(g, adc);		 // verde
		pwm_rojo = apply_brightness(r, adc); // rojo por software
	}
}