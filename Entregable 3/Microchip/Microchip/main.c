/*
 * Microchip.c
 *
 * Created: 2/6/2025 14:40:23
 * Author : Barcala
 */ 

#include <avr/io.h>

void i2c_init(void){
	TWSR= 0x00;
	TWBR= 72; //scl frecuecnia 100kHz
	TWCR= 0x04; 
}

void i2c_start(void){
	TWCR=(1<< TWINT) | (1 << TWSTA) | (1<< TWEN);
	while((TWCR & (1<<TWINT)) == 0); 
}

void i2c_stop(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO); 
}

void i2c_write(unsigned char data)
{
	TWDR = data;
	TWCR =(1<<TWINT) | (1<<TWEN);
	while((TWCR & (1<<TWINT)) == 0);
}

unsigned char i2c_read(unsigned char isLast)
{
	if (isLast == 0) //send ACK
		TWCR= (1<< TWINT)|(1<<TWEN) | (1<<TWEA); //send ACK
	else
		TWCR =(1<< TWINT) | (1<<TWEN); //send NACK
	while ((TWCR & (1<<TWINT)) == 0);
	return TWDR;

}

int main(void)
{
    i2c_init();
	i2c_start();
	i2c_write(0xD0);
	i2c_write(0);
	i2c_write(10);
	i2c_write(11);
	i2c_write(12);
	i2c_stop();
    while (1) 
    {
		asm("nop");
    }
}

