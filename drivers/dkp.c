/***************************************************************************************************************************************************
* Author         : Labhesh Patil
* Date           : December 6, 2025
* File           : dkp.c
* Title          : Digital keypad driver
* Description    : Handles digital keypad input with two detection modes:
*                 LEVEL mode for continuous reading, STATE mode for edge-triggered.
****************************************************************************************************************************************************/

#include <xc.h>
#include "dkp.h"

void init_digital_keypad(void)
{
    /* Set Keypad Port as input */
    KEYPAD_PORT_DDR = KEYPAD_PORT_DDR | INPUT_LINES;
}

unsigned char read_digital_keypad(unsigned char mode)
{
    static unsigned char once = 1;
    
    if (mode == LEVEL_DETECTION)
    {
        return KEYPAD_PORT & INPUT_LINES;
    }
    else
    {
        if (((KEYPAD_PORT & INPUT_LINES) != ALL_RELEASED) && once)
        {
            once = 0;
            
            return KEYPAD_PORT & INPUT_LINES;
        }
        else if ((KEYPAD_PORT & INPUT_LINES) == ALL_RELEASED)
        {
            once = 1;
        }
    }
    
    return ALL_RELEASED;
}

