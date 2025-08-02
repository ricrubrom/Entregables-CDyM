#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef TIMER0_H
#define TIMER0_H

extern uint8_t pwm_rojo;
void timer0_init(void);

#endif