/**************************************************************************************************
* Author         : Labhesh Patil
* Date           : December 7, 2025
* File           : dashboard.c
* Title          : Main dashboard display
* Description    : Displays current time from RTC, gear position, and speed from ADC.
*                 Handles gear up/down (SW2/SW3) and collision detection (SW1).
*                 Logs events to external EEPROM when keys are pressed.
****************************************************************************************************/
#include <xc.h>
#include "main.h"

unsigned char clock_reg[3];
unsigned char pos = 0;
unsigned short adc_value;
unsigned char speed[3] = {0};
const char *event[] = {"ON", "GN", "G1", "G2", "G3", "G4", "G5", "GR", "C "};

void dashboard(unsigned char key) 
{
    // display dashboard headings
    clcd_print("  TIME    EV  SP", LINE1(0));

    // display current time from RTC
    get_time(clock_reg);
    display_time(clock_reg, 0);

    // collision detection
    if (key == SW1) 
    {
        pos = 8;
    }
    // increment gear (SW2 press)
    else if (key == SW2) 
    {
        if (pos < 7) 
        {
            ++pos;
        }
    }
    // decrement gear (SW3 press)
    else if (key == SW3) 
    {
        if (pos > 1) 
        {
            --pos;
        }
    }

    // display current event
    clcd_print(event[pos], LINE2(10));

    // Read and convert speed from ADC
    adc_value = read_adc();
    adc_value = (adc_value) / 10.23;       // convert to 0-100 range

    // convert speed to ascii characters
    speed[1] = (adc_value % 10) + '0';
    speed[0] = (adc_value / 10) + '0';

    // display speed
    clcd_print(speed, LINE2(14));

    // store event in EEPROM when any event occurs (mimic using key press)
    if (key == SW1 || key == SW2 || key == SW3) 
    {
        store_event(clock_reg, event[pos], speed);
    }
}