/*
 * main.h
 *
 * Created: 6/6/2025 16:04:59
 *  Author: mater
 */

#ifndef MAIN_H
#define MAIN_H

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

#define BUFFER_SIZE 64

#endif /* MAIN_H_ */