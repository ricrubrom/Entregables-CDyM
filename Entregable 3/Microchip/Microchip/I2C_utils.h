/*
 * I2C_utils.h
 *
 * Created: 6/6/2025 14:53:35
 *  Author: mater
 */

#ifndef I2C_UTILS_H_
#define I2C_UTILS_H_

#include "main.h"

#define I2C_START_CMD ((1 << TWINT) | (1 << TWSTA) | (1 << TWEN))
#define I2C_STOP_CMD ((1 << TWINT) | (1 << TWEN) | (1 << TWSTO))
#define I2C_WRITE_CMD ((1 << TWINT) | (1 << TWEN))
#define I2C_READ_ACK ((1 << TWINT) | (1 << TWEN) | (1 << TWEA))
#define I2C_READ_NACK ((1 << TWINT) | (1 << TWEN))

void I2C_init(void);
void I2C_start(void);
void I2C_stop(void);
void I2C_write(uint8_t data);
uint8_t I2C_read(uint8_t isLast);

#endif /* I2C_UTILS_H_ */