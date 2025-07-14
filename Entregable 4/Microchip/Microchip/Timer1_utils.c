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
  //setea Fast PWM, 8-bit (non-inverted) con prescaler 8.
  TCCR1B = (1 << WGM12) | (1<<CS11);
  TCCR1A = (1<<COM1A1) | (1<<COM1B1) | (1<<WGM10);
  //freq PWM = 16Mhz/(8*256) = 7812.5Hz > 50Hz

  OCR1A = 255;
  OCR1B = 255;
}
