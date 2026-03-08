/*******************************************************************************************
* Author         : Labhesh Patil
* Date           : December 7, 2025
* File           : login.c
* Title          : Login screen handler
* Description    : Implements the password entry screen with cursor blinking, attempt
*                 tracking, and 2 minute blocking after 3 failed attempts. Uses SW4
*                 for 'x' and SW5 for 'y' password characters.
***********************************************************************************************/
#include <xc.h>
#include "main.h"
#include "config.h"

// password storage
char correct_pass[PASS_LEN];
char user_pass[PASS_LEN];

// login state variables
unsigned int attempts = MAX_ATTEMPTS;
unsigned int index = 0;
unsigned int blink_freq = 0;
unsigned char blink_toggle = 0;

// blocking
unsigned char is_blocked = 0;

unsigned char key;
unsigned int timeout = 0;
unsigned char disp_once = 1;

// compare two passwords
int compare_pass(const char *p1, const char *p2) 
{
    for (int i = 0; i < PASS_LEN; i++)
    {
        if (p1[i] != p2[i])
            return FAILURE;
    }
    return SUCCESS;
}

// login screen
unsigned char login_screen(unsigned char login_mode) 
{
    // handle blocked state block for 2 minutes
    if (is_blocked) 
    {
        clcd_print(" LOCKED 2 MIN   ", LINE1(0));
        clcd_print(" TRY LATER      ", LINE2(0));

        // 2 minute delay -> 6 sec x 20
        for (int i = 0; i < 20; i++) 
        {
            __delay_ms(6000);
        }

        // reset after block period
        is_blocked = 0;
        attempts = MAX_ATTEMPTS;
        index = 0;
        timeout = 0;
        disp_once = 1;

        return login_mode;
    }

    // display prompt once
    if (disp_once) 
    {
        clcd_clear();
        clcd_print("ENTER PASSWORD  ", LINE1(0));
        clcd_print("                ", LINE2(0));
        disp_once = 0;
    }

    // handle cursor blinking
    if (index < PASS_LEN) 
    {
        if (blink_freq++ >= BLINK_FREQ_DIVIDER) 
        {
            blink_freq = 0;
            clcd_putch(blink_toggle ? '_' : 255, LINE2(index));
            blink_toggle ^= 1;
        }
    }

    // read keypad input
    key = read_digital_keypad(STATE);

    if (key != ALL_RELEASED) 
    {
        timeout = 0;

        if (index < PASS_LEN) 
        {
            // mapping keys to password characters
            if (key == SW4)
                user_pass[index] = 'x';
            else if (key == SW5)
                user_pass[index] = 'y';
            else
                return login_mode;

            clcd_putch('*', LINE2(index));
            index++;

            blink_toggle = 0;
            blink_freq = 0;

            __delay_ms(200); 
        }
    }

    // check password when fully entered
    if (index == PASS_LEN) 
    {
        blink_toggle = 0;

        if (compare_pass(user_pass, correct_pass) == SUCCESS) 
        {
            clcd_print("CORRECT PASSWORD", LINE1(0));
            clcd_print("LOGIN SUCCESSFUL", LINE2(0));
            __delay_ms(2000);

            // clear clcd
            clcd_clear();

            // reset login state
            index = 0;
            disp_once = 1;
            timeout = 0;
            blink_freq = 0;
            blink_toggle = 0;
            attempts = MAX_ATTEMPTS;
            for (int i = 0; i < PASS_LEN; i++) 
                user_pass[i] = 0;

            return MODE_MENU;
        } 
        else 
        {
            attempts--;
            index = 0;
            disp_once = 1;
            blink_freq = 0;
            blink_toggle = 0;

            clcd_print(" WRONG PASSWORD ", LINE1(0));
            clcd_print("Attempts Left :  ", LINE2(0));
            clcd_putch(attempts + '0', LINE2(15));
            
            // clear entered password
            for (int i = 0; i < PASS_LEN; i++) 
                user_pass[i] = 0;

            __delay_ms(2000);

            if (attempts == 0)
                is_blocked = 1;
        }
    }

    // automatic timeout after 5 seconds if no input happens
    timeout++;
    if (timeout > TIMEOUT_5SEC) 
    {
        login_mode = MODE_DASHBOARD;
        timeout = 0;
        index = 0;
        blink_freq = 0;
        blink_toggle = 0;
        attempts = MAX_ATTEMPTS;
        disp_once = 1;

        // clear entered password
        for (int i = 0; i < PASS_LEN; i++) 
            user_pass[i] = 0;

        return MODE_DASHBOARD;
    }

    __delay_us(100);
    return login_mode;
}