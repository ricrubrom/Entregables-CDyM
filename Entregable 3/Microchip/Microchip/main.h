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

typedef enum
{
  DAY,
  MONTH,
  YEAR,
  HOUR,
  MIN,
  SEC,
} RTC_Field;

#define BUFFER_SIZE 128

#endif /* MAIN_H_ */