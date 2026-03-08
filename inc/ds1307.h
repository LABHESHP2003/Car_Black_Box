/***************************************************************************************************************************************************
* Author         : Labhesh Patil
* Date           : December 6, 2025
* File           : ds1307.h
* Title          : RTC header file
* Description    : I2C slave addresses and register addresses for DS1307 RTC.
*                 Function declarations for RTC operations.
****************************************************************************************************************************************************/

#ifndef DS1307_H
#define	DS1307_H

#define SLAVE_WRITE             0b11010000 // 0xD0
#define SLAVE_READ              0b11010001 // 0xD1

#define SEC_ADDR                0x00
#define MIN_ADDR                0x01
#define HOUR_ADDR               0x02


void init_ds1307(void);
unsigned char read_ds1307(unsigned char addr);
void write_ds1307(unsigned char addr, unsigned char data);
void get_time(unsigned char *clock_reg);
void display_time(unsigned char *clock_reg, unsigned int index);

#endif	/* DS1307_H */
