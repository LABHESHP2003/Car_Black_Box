/***************************************************************************************************************************************************
* Author         : Labhesh Patil
* Date           : December 6, 2025
* File           : i2c.h
* Title          : I2C header file
* Description    : Function declarations for I2C operations and crystal frequency
*                 definition for baud rate calculation.
****************************************************************************************************************************************************/
#ifndef I2C_H
#define	I2C_H

#define FOSC            20000000

void init_i2c(unsigned long baud);
void i2c_start(void);
void i2c_rep_start(void);
void i2c_stop(void);
unsigned char i2c_read(unsigned char ack);
int i2c_write(unsigned char data);

#endif	/* I2C_H */


