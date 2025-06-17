/*
 * Timer1_utils.c
 *
 * Created: 9/6/2025 15:24:20
 *  Author: mater
 */

#include "Timer1_utils.h"

ISR(TIMER1_COMPA_vect)
{
  time_flag = true;
}

void Timer1_init()
{
  // Modo CTC: WGM12 = 1, WGM13:WGM10 = 0
  TCCR1B |= (1 << WGM12);
  TCCR1A &= ~((1 << WGM11) | (1 << WGM10));

  // Set prescaler a 1024: CS12 y CS10 = 1
  TCCR1B |= (1 << CS12) | (1 << CS10);
  TCCR1B &= ~(1 << CS11);

  // Valor para OCR1A para 1 segundo
  OCR1A = 15624;

  // Habilitar interrupción por comparación A
  TIMSK1 |= (1 << OCIE1A);

  // Inicializar el contador
  TCNT1 = 0;
}
