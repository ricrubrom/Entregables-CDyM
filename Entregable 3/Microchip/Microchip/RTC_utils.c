/*
 * RTC_utils.c
 *
 * Created: 6/6/2025 14:58:46
 *  Author: mater
 */

#include "RTC_utils.h"
#include "I2C_utils.h"

void RTC_Init(void)
{
  I2C_init();
  I2C_start();

  I2C_write(RTC_WriteMode); // Dirección de escritura del RTC
  I2C_write(RTC_ControlRegAddress);
  I2C_write(0x00); // Configurar el registro de control (deshabilitar alarmas, etc.)

  I2C_stop();
}

void RTC_SetTime(RTC_t rtc)
{
  I2C_start();

  I2C_write(RTC_WriteMode);
  I2C_write(RTC_SecondRegAddress);
  I2C_write(InttoBCD(rtc.sec) & SEC_MASK);
  I2C_write(InttoBCD(rtc.min) & MIN_MASK);
  I2C_write(InttoBCD(rtc.hour) & HOUR_MASK);

  I2C_stop();

  I2C_start();

  I2C_write(RTC_WriteMode);
  I2C_write(RTC_DaysRegAddress);
  I2C_write(InttoBCD(rtc.day) & DAY_MASK);
  I2C_write(InttoBCD(rtc.month) & MONTH_MASK);
  I2C_write(InttoBCD(rtc.year) & YEAR_MASK);

  I2C_stop();
}

RTC_t RTC_GetTime(void)
{
  RTC_t rtc;
  I2C_start();

  I2C_write(RTC_WriteMode);
  I2C_write(RTC_SecondRegAddress);

  I2C_stop();

  I2C_start();

  I2C_write(RTC_ReadMode);
  rtc.sec = BCDtoInt(I2C_read(false));
  rtc.min = BCDtoInt(I2C_read(false));
  rtc.hour = BCDtoInt(I2C_read(true));

  I2C_stop();

  I2C_start();

  I2C_write(RTC_WriteMode);
  I2C_write(RTC_DaysRegAddress);

  I2C_stop();

  I2C_start();

  I2C_write(RTC_ReadMode);
  rtc.day = BCDtoInt(I2C_read(false));
  rtc.month = BCDtoInt(I2C_read(false));
  rtc.year = BCDtoInt(I2C_read(true));

  I2C_stop();

  return rtc;
}

// RTC define el tiempo con BCD, por ende es necesaria la traduccion
uint8_t BCDtoInt(uint8_t value)
{
  return ((value >> 4) * 10) + (value & 0x0F);
}

uinty_t InttoBCD(uint8_t)
{
  return ((value / 10) << 4) | (value % 10);
}
