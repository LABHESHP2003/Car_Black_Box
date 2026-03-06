/* 
 * File:   ext_eeprom.h
 * Author: labhe
 *
 * Created on December 18, 2025, 12:19 AM
 */

/* 
 * File:   eeprom.h
 * Author: labhesh
 *
 * Created on December 5, 2025, 11:23 AM
 */

#ifndef EEPROM_H
#define	EEPROM_H
//
#define EEPROM_SLAVE_WRITE             0b10100000 // 0xA0
#define EEPROM_SLAVE_READ              0b10100001 // 0xA1

void init_ext_eeprom(void);
unsigned char read_ext_eeprom(unsigned char addr);
void write_ext_eeprom(unsigned char addr, unsigned char data);

#endif	/* EEPROM_H */