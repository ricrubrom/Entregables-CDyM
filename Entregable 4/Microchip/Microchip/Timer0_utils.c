#include <stdio.h>
#include <string.h>
#include "Timer0_utils.h"

void timer0_init(void)
{
	TCCR0A = (1 << WGM21);	// CTC mode
	TCCR0B = (1 << CS21);		// prescaler 8
	OCR0A = 63;							// genera interrupción cada 256 ciclos / 4 = 64 (31.25 kHz > 50Hz)
	TIMSK0 = (1 << OCIE0A); // habilito interrupción
}

ISR(TIMER0_COMPA_vect)
{
	static uint8_t counter = 0;
	counter++;
	if (counter >= 255)
		counter = 0;

	if (counter < pwm_rojo)
		PORTB |= (1 << PB5); // led apagado
	else
		PORTB &= ~(1 << PB5); // led encendido
}
