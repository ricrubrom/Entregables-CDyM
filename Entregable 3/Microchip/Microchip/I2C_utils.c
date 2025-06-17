/*
 * I2C_utils.c
 *
 * Created: 6/6/2025 14:53:13
 *  Author: mater
 */

#include "I2C_utils.h"

void I2C_init(void)
{
  TWSR = 0x00;
  TWBR = 72; // scl frecuencia 100kHz
  TWCR = 0x04;
}

void I2C_start(void)
{
  TWCR = I2C_START_CMD;
  while ((TWCR & (1 << TWINT)) == 0)
    ;
}

void I2C_stop(void)
{
  TWCR = I2C_STOP_CMD;
}

void I2C_write(unsigned char data)
{
  TWDR = data;
  TWCR = I2C_WRITE_CMD;
  while ((TWCR & (1 << TWINT)) == 0)
    ;
}

uint8_t I2C_read(unsigned char isLast)
{
  if (isLast == 0)
    TWCR = I2C_READ_ACK;
  else
    TWCR = I2C_READ_NACK;
  while ((TWCR & (1 << TWINT)) == 0)
    ;
  return TWDR;
}
