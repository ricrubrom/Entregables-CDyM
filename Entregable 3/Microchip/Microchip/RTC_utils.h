/*
 * RTC_utils.h
 *
 * Created: 6/6/2025 14:58:59
 *  Author: mater
 */

#ifndef RTC_UTILS_H_
#define RTC_UTILS_H_

#include <avr/io.h>
#include "main.h"

extern volatile bool alarm;

typedef struct
{
  uint8_t seconds; // 0-59
  uint8_t minutes; // 0-59
  uint8_t hours;   // 0-23
  uint8_t day;     // 1-31
  uint8_t month;   // 1-12
  uint16_t year;   // Year since 2000
} RTC_t;

#define SEC_MASK 0b01111111
#define MIN_MASK 0b01111111
#define HOUR_MASK 0b00111111
#define DAY_MASK 0b00111111
#define MONTH_MASK 0b00011111
#define YEAR_MASK 0b11111111

#define RTC_ControlRegAddress 0x68 // Dirección de Control del RTC DS3231
// Para leer o escribir en el módulo, se debe utilizar la dirección de control y poner el último bit en 0 o 1 según lectura o escritura
// Los siguientes son los bytes para leer o escribir, con la dirección destino y la operación a realizar
#define RTC_ReadMode (RTC_ControlRegAddress << 1) | 0x01
#define RTC_WriteMode (RTC_ControlRegAddress << 1)

#define RTC_SecondRegAddress 0x00 // Minutos y horas en celdas siguientes, no es necesario declararlas porque al escribir se mueven solas

#define RTC_DaysRegAddress 0x04 // Meses y Anios en celdas siguientes, no es necesario declararlas porque al escribir se mueven solas

#define RTC_Alarm1RegAddress 0x07

#define RTC_Alarm1InterruptEnable 0b00000101

void RTC_Init();
void RTC_GetTime(RTC_t *rtc);
void RTC_SetTime(RTC_t *rtc);
void RTC_SetAlarm(RTC_t rtc);

#endif /* RTC_UTILS_H_ */