/*
 * I2C_utils.c
 *
 * Created: 6/6/2025 14:53:13
 *  Author: mater
 */

#include "I2C_utils.h"

void i2c_init(void)
{
  TWSR = 0x00;
  TWBR = 72; // scl frecuecnia 100kHz
  TWCR = 0x04;
}

void i2c_start(void)
{
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  while ((TWCR & (1 << TWINT)) == 0)
    ;
}

void i2c_stop(void)
{
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void i2c_write(unsigned char data)
{
  TWDR = data;
  TWCR = (1 << TWINT) | (1 << TWEN);
  while ((TWCR & (1 << TWINT)) == 0)
    ;
}

unsigned char i2c_read(unsigned char isLast)
{
  if (isLast == 0)                                   // send ACK
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA); // send ACK
  else
    TWCR = (1 << TWINT) | (1 << TWEN); // send NACK
  while ((TWCR & (1 << TWINT)) == 0)
    ;
  return TWDR;
}