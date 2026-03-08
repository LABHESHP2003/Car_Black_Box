/***************************************************************************************************************************************************
* Author         : Labhesh Patil
* Date           : December 18, 2025
* File           : ext_eeprom.c
* Title          : External EEPROM driver
* Description    : I2C-based driver for external EEPROM.
*                 Provides byte read/write operations with proper delay for
*                 write cycle completion.
****************************************************************************************************************************************************/

#include <xc.h>
#include "i2c.h"
#include "ext_eeprom.h"

unsigned char read_ext_eeprom(unsigned char addr)
{
    unsigned char data;
    
    i2c_start();
    i2c_write(EEPROM_SLAVE_WRITE);
    i2c_write(addr);
    i2c_rep_start();
    i2c_write(EEPROM_SLAVE_READ);
    data = i2c_read(0);
    i2c_stop();
    
    return data;
}

void write_ext_eeprom(unsigned char addr, unsigned char data) // SEc_ADDR, data
{
    i2c_start();
    i2c_write(EEPROM_SLAVE_WRITE);
    i2c_write(addr);
    i2c_write(data);
    i2c_stop();
    for(int delay = 0; delay < 100; delay++);
}



