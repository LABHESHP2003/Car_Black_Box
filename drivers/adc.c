/***************************************************************************************************************************************************
* Author         : Labhesh Patil
* Date           : December 6, 2025
* File           : adc.c
* Title          : ADC module driver
* Description    : Initializes and controls the Analog-to-Digital Converter module.
*                 Reads analog value from potentiometer (channel 0) and returns
*                 10-bit digital value (0-1023) for speed calculation.
****************************************************************************************************************************************************/

#include <xc.h>
#include "adc.h"

void init_adc(void)
{
    /* Selecting Right Justification */
    ADFM = 1;
    
    /* Starting the ADC Module */
    ADON = 1; 
    
    ADCS2 = 0;
    ADCS1 = 1;
    ADCS0 = 0;
    
    CHS2 = 0;
    CHS1 = 0;
    CHS0 = 0;

    
    PCFG3 = 0;
    PCFG2 = 0;
    PCFG1 = 0;
    PCFG0 = 0;
    
    ADRESH = 0x00;
    ADRESL = 0x00;
    
//    TRISA0 = 1;
}

unsigned short read_adc(void)
{
    unsigned short adc_reg_val; // 2bytes
    
    /* Start the ADC conversion */
    GO = 1;
    
    /* Wait for the conversion to complete */
    while (nDONE); //nDONE = 0 GO = 0
    
   
    adc_reg_val = (ADRESH << 8) | ADRESL;  // 11 1111 1111 -> 1023
    
    return adc_reg_val; // 0 to 1023
    
}

