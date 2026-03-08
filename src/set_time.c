/****************************************************************************************************
* Author         : Labhesh Patil
* Date           : December 18, 2025
* File           : set_time.c
* Title          : RTC time setting
* Description    : Allows user to set RTC time with field selection and blinking cursor.
*                 SW4 decreases current field value, SW5 increases. Long press SW5
*                 moves to next field, long press on last field saves to RTC.
*                 Long press SW4 cancels and returns to menu.
******************************************************************************************************/

#include <xc.h>
#include "main.h"
#include "config.h"

// blink cursor variables
unsigned int blink_freq = 0;
unsigned char blink_toggle = 0;

// convert decimal to BCD format for RTC
unsigned char dec_to_bcd(unsigned char dec)
{
    return ((dec/10) << 4) | (dec % 10);
}

// time setting
void set_time_screen(void)
{
    unsigned char key;
    unsigned char field = 0;// 0=hour, 1=min, 2=sec
    unsigned int sw4_time = 0;
    unsigned char reset4 = 0;
    unsigned int sw5_time = 0;
    unsigned char reset5 = 0;
    unsigned char clock_reg[3];

    clcd_clear();

    //get current time from RTC
    get_time(clock_reg);  

    while (1)
    {
        key = read_digital_keypad(LEVEL);

        // display headings
        clcd_print("SET     ", LINE1(0));
        clcd_print("TIME    ", LINE2(0));

        // handle blinking cursor for current field
        if (blink_freq++ >= 3) 
        {
            blink_freq = 0;
            
            if (field == 0)
            {
                // blink hours field
                clcd_putch(blink_toggle ? ' ' : (clock_reg[0] >> 4) + '0', LINE2(8));
                clcd_putch(blink_toggle ? ' ' : (clock_reg[0] & 0x0F) + '0', LINE2(9));
            }
            else if (field == 1)
            {
                // blink minutes field
                clcd_putch(blink_toggle ? ' ' : (clock_reg[1] >> 4) + '0', LINE2(11));
                clcd_putch(blink_toggle ? ' ' : (clock_reg[1] & 0x0F) + '0', LINE2(12));
            }
            else if (field == 2)
            {
                // blink seconds field
                clcd_putch(blink_toggle ? ' ' : (clock_reg[2] >> 4) + '0', LINE2(14));
                clcd_putch(blink_toggle ? ' ' : (clock_reg[2] & 0x0F) + '0', LINE2(15));
            }
            blink_toggle ^= 1;
        }
        else
        {
            // normal display without blink
            display_time(clock_reg, 8);
        }

        //SW4 (Decrease / Cancel)
        if (key == SW4)
        {
            sw4_time++;

            // long press-> cancel and return back to menu
            if (sw4_time > LONG_PRESS_THRESHOLD && reset4 == 0)
            {
                reset4 = 1;
                clcd_clear();
                return;
            }
        }
        else
        {
            // short press-> decrease current field value
            if (sw4_time > 0 && sw4_time <= SHORT_PRESS_THRESHOLD && reset4 == 0)
            {
                unsigned char value;

                if (field == 0)
                {
                    value = ((clock_reg[0] >> 4) * 10) + (clock_reg[0] & 0x0F);
                    value = (value == 0) ? 23 : value - 1;
                    clock_reg[0] = dec_to_bcd(value);
                }
                else if (field == 1)
                {
                    value = ((clock_reg[1] >> 4) * 10) + (clock_reg[1] & 0x0F);
                    value = (value == 0) ? 59 : value - 1;
                    clock_reg[1] = dec_to_bcd(value);
                }
                else if (field == 2)
                {
                    value = ((clock_reg[2] >> 4) * 10) + (clock_reg[2] & 0x0F);
                    value = (value == 0) ? 59 : value - 1;
                    clock_reg[2] = dec_to_bcd(value);
                }
            }

            sw4_time = 0;
            reset4 = 0;
        }

        //SW5 (Increase / Set and move cursor to next field)
        if (key == SW5)
        {
            sw5_time++;

            // long press->move to next field or save
            if (sw5_time > LONG_PRESS_THRESHOLD && reset5 == 0)
            {
                reset5 = 1;
                field++;

                if (field > 2)
                {
                    // save all fields to RTC
                    write_ds1307(HOUR_ADDR, clock_reg[0]);
                    write_ds1307(MIN_ADDR,  clock_reg[1]);
                    write_ds1307(SEC_ADDR,  clock_reg[2] & 0x7F);

                    clcd_clear();
                    clcd_print(" TIME UPDATED   ", LINE1(0));
                    __delay_ms(1500);
                    return;
                }
            }
        }
        else
        {
            // short press -> increase current field value
            if (sw5_time > 0 && sw5_time <= SHORT_PRESS_THRESHOLD && reset5 == 0)
            {
                unsigned char value;

                if (field == 0)
                {
                    value = ((clock_reg[0] >> 4) * 10) + (clock_reg[0] & 0x0F);
                    value = (value + 1) % 24;
                    clock_reg[0] = dec_to_bcd(value);
                }
                else if (field == 1)
                {
                    value = ((clock_reg[1] >> 4) * 10) + (clock_reg[1] & 0x0F);
                    value = (value + 1) % 60;
                    clock_reg[1] = dec_to_bcd(value);
                }
                else if (field == 2)
                {
                    value = ((clock_reg[2] >> 4) * 10) + (clock_reg[2] & 0x0F);
                    value = (value + 1) % 60;
                    clock_reg[2] = dec_to_bcd(value);
                }
            }

            sw5_time = 0;
            reset5 = 0;
        }
    }
}