/***************************************************************************************************************************************************
* Author         : Labhesh Patil
* Date           : December 6, 2025
* File           : dkp.h
* Title          : Digital keypad header
* Description    : Keypad port definitions, key mask values, and mode definitions.
*                 Defines SW1 through SW6 with their corresponding bit patterns.
****************************************************************************************************************************************************/

#ifndef DKP_H
#define	DKP_H

#define LEVEL_DETECTION         0
#define LEVEL                   0

#define STATE_DETECTION         1
#define STATE                   1

#define KEYPAD_PORT             PORTB
#define KEYPAD_PORT_DDR         TRISB

#define INPUT_LINES             0x3F

#define SW1                     0x3E
#define SW2                     0x3D
#define SW3                     0x3B
#define SW4                     0x37
#define SW5                     0x2F
#define SW6                     0x1F

#define ALL_RELEASED            INPUT_LINES

unsigned char read_digital_keypad(unsigned char mode);
void init_digital_keypad(void);


#endif	/* DKP_H */

