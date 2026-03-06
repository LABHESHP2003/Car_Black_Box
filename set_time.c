/*
 * File:   set_time.c
 * Author: labhe
 *
 * Created on March 2, 2026, 6:35 PM
 */


#include <xc.h>
#include "main.h"

unsigned int blink_freq = 0;
unsigned char blink_toggle = 0;

unsigned char dec_to_bcd(unsigned char dec){
    return ((dec/10) << 4)|(dec % 10);
}

void set_time(unsigned char hour, unsigned char min, unsigned char sec){
    //conversion to bcd
    sec  = dec_to_bcd(sec) & 0x7F;
    min  = dec_to_bcd(min);
    hour = dec_to_bcd(hour);
    
    write_ds1307(SEC_ADDR, sec);
    write_ds1307(MIN_ADDR, min);
    write_ds1307(HOUR_ADDR, hour);
}

void set_time_screen(void)
{
    unsigned char key;
    unsigned int field = 0;
    unsigned int sw4_time = 0;
    unsigned int reset4 = 0;
    unsigned int sw5_time = 0;
    unsigned int reset5 = 0;
    unsigned char clock_reg[3];

    clcd_clear();

    // Get current time from RTC
    get_time(clock_reg);  

    while (1)
    {
        key = read_digital_keypad(LEVEL);

        //clcd_print("SET TIME        ", LINE1(0));
        clcd_print("SET             ", LINE1(0));
        clcd_print("TIME    ", LINE2(0));


        // Display using your existing function
        //display_time(clock_reg,8);

        /* ---------- Cursor indication ---------- */
//        if (field == 0)
//            clcd_putch('^', LINE1(0+8));   // above HH
//        else if (field == 1)
//            clcd_putch('^', LINE1(3+8));   // above MM
//        else if (field == 2)
//            clcd_putch('^', LINE1(6+8));   // above SS
        
        if (blink_freq++ >= 3) {
            blink_freq = 0;
            if (field == 0){
            clcd_putch(blink_toggle ? ' ' : clock_reg[0]/10, LINE2(8));
            clcd_putch(blink_toggle ? ' ' : clock_reg[0]%10, LINE2(9));
            }
            else if (field == 1){
                clcd_putch(blink_toggle ? ' ' : clock_reg[1]/10, LINE2(11));
                clcd_putch(blink_toggle ? ' ' : clock_reg[1]%10, LINE2(12));
            }
            else if (field == 2){
                clcd_putch(blink_toggle ? ' ' : clock_reg[2]/10, LINE2(14));
                clcd_putch(blink_toggle ? ' ' : clock_reg[2]%10, LINE2(15));
            }
            blink_toggle ^= 1;
        }
        else{
            display_time(clock_reg,8);
        }


        /* ========== SW4 (Increase / Cancel) ========== */

        if (key == SW4)
        {
            sw4_time++;

            // LONG PRESS ? Cancel
            if (sw4_time > 25 && reset4 == 0)
            {
                reset4 = 1;
                clcd_clear();
                return;     // back to menu
            }
        }
        else
        {
            // SHORT PRESS ? Increase
            if (sw4_time > 0 && sw4_time <= 5 && reset4 == 0)
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


        /* ========== SW5 (Decrease / Next Field) ========== */

        if (key == SW5)
        {
            sw5_time++;

            // LONG PRESS ? Next field / Save
            if (sw5_time > 25 && reset5 == 0)
            {
                reset5 = 1;
                field++;

                if (field > 2)
                {
                    // SAVE to RTC
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
            // SHORT PRESS ? Decrease
            if (sw5_time > 0 && sw5_time <= 5 && reset5 == 0)
            {
                unsigned char value;

                if (field == 0)          // HH
                {
                    value = ((clock_reg[0] >> 4) * 10) + (clock_reg[0] & 0x0F);
                    value = (value + 1) % 24;
                    clock_reg[0] = dec_to_bcd(value);
                }
                else if (field == 1)     // MM
                {
                    value = ((clock_reg[1] >> 4) * 10) + (clock_reg[1] & 0x0F);
                    value = (value + 1) % 60;
                    clock_reg[1] = dec_to_bcd(value);
                }
                else if (field == 2)     // SS
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