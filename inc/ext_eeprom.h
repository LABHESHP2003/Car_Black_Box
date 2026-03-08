/***************************************************************************************************************************************************
* Author         : Labhesh Patil
* Date           : December 18, 2025
* File           : ext_eeprom.h
* Title          : External EEPROM header
* Description    : I2C slave addresses for external EEPROM and function declarations.
****************************************************************************************************************************************************/
#ifndef EEPROM_H
#define	EEPROM_H
//
#define EEPROM_SLAVE_WRITE             0b10100000 // 0xA0
#define EEPROM_SLAVE_READ              0b10100001 // 0xA1

void init_ext_eeprom(void);
unsigned char read_ext_eeprom(unsigned char addr);
void write_ext_eeprom(unsigned char addr, unsigned char data);

#endif	/* EEPROM_H */