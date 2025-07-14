/*
 * main.h
 *
 * Created: 6/6/2025 16:04:59
 *  Author: mater
 */

#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#define BR9600 (0x67)

typedef enum
{
  false,
  true
} bool;

#define BUFFER_SIZE 256

#endif /* MAIN_H_ */