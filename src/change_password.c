/******************************************************************************************
* Author         : Labhesh Patil
* Date           : December 18, 2025
* File           : change_password.c
* Title          : Password change handler
* Description    : Two step password change process - first verify old password,
*                 then enter new password. New password is stored in internal EEPROM.
*                 Includes attempt tracking, timeout after 5 seconds inactivity,
*                 and 2-minute blocking after 3 failed attempts.
********************************************************************************************/
#include <xc.h>
#include "main.h"
#include "config.h"

// User input buffer
char user_pass[PASS_LEN];

// login state variable
unsigned int attempts = MAX_ATTEMPTS;
unsigned int index = 0;
unsigned int blink_freq = 0;
unsigned char blink_toggle = 0;

// blocking
unsigned char is_blocked = 0;

unsigned char key;
unsigned int timeout = 0;
unsigned char disp_once = 1;

// password change
int change_password() 
{
    unsigned int ret_old_pass = 0, ret_new_pass = 0;
    
    // Step 1: Verify old password
    while ((ret_old_pass = old_password()) == CONTINUE) 
    {
        // wait for password entry
    }

    // Step 2: If old password correct, set new password
    if (ret_old_pass == SUCCESS) 
    {
        while((ret_new_pass = new_password()) == CONTINUE)
        {
            // Wait for new password entry
        }
        
        if(ret_new_pass == SUCCESS)
        {
            return SUCCESS;
        }
    }
    
    return SUCCESS;
}

// verify old password before allowing any changes
unsigned int old_password() 
{
    // Display prompt once
    if (disp_once) 
    {
        clcd_print(" ENTER OLD PASS ", LINE1(0));
        clcd_print("                ", LINE2(0));
        disp_once = 0;
        __delay_ms(1000);
    }

    // Handle cursor blinking
    if (index < PASS_LEN) 
    {
        if (blink_freq++ >= BLINK_FREQ_DIVIDER) 
        {
            blink_freq = 0;
            clcd_putch(blink_toggle ? '_' : 255, LINE2(index));
            blink_toggle ^= 1;
        }
    }

    // Read keypad input
    key = read_digital_keypad(STATE);

    if (key != ALL_RELEASED) 
    {
        timeout = 0;

        if (index < PASS_LEN) 
        {
            // key mapping for password character
            if (key == SW4)
                user_pass[index] = 'x';
            else if (key == SW5)
                user_pass[index] = 'y';
            else
                return CONTINUE;

            clcd_putch('*', LINE2(index));
            index++;

            // reset blink state
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

            // Clear clcd
            clcd_clear();

            // eeset login state
            index = 0;
            disp_once = 1;
            timeout = 0;
            blink_freq = 0;
            blink_toggle = 0;
            attempts = MAX_ATTEMPTS;
            for (int i = 0; i < PASS_LEN; i++) 
                user_pass[i] = 0;

            return SUCCESS;
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

    // automatic timeout if no input for 5 seconds
    timeout++;
    if (timeout > TIMEOUT_5SEC) 
    {
        timeout = 0;
        index = 0;
        blink_freq = 0;
        blink_toggle = 0;
        attempts = MAX_ATTEMPTS;
        disp_once = 1;

        // Clear entered password
        for (int i = 0; i < PASS_LEN; i++) 
            user_pass[i] = 0;
            
        return TIMEOUT;
    }

    __delay_us(100);
    return CONTINUE;
}

// Enter new password
unsigned int new_password() 
{
    // Display prompt for input
    if (disp_once) 
    {
        clcd_print(" ENTER NEW PASS ", LINE1(0));
        clcd_print("                ", LINE2(0));
        disp_once = 0;
        __delay_us(1000);
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
                return CONTINUE;

            clcd_putch('*', LINE2(index));
            index++;

            // reset blink state
            blink_toggle = 0;
            blink_freq = 0;

            __delay_ms(200);
        }
    }

    // Save new password when fully entered
    if (index == PASS_LEN) 
    {
        blink_toggle = 0;

        clcd_print(" PASSWORD CHANGED", LINE1(0));
        clcd_print("   SUCCESSFULLY  ", LINE2(0));
        __delay_ms(2000);

        // Clear clcd
        clcd_clear();

        // Store new password in EEPROM
        eeprom_write(0x00, 0xAA);
        unsigned char i = 0x01;
        for (int k = 0; k < PASS_LEN; k++) 
        {
            eeprom_write(i++, user_pass[k]);
        }
        
        // reload password into memory
        init_password();

        // reset login state
        index = 0;
        disp_once = 1;
        timeout = 0;
        blink_freq = 0;
        blink_toggle = 0;
        for (int i = 0; i < PASS_LEN; i++) 
            user_pass[i] = 0;

        return SUCCESS;
    }

    // auto timeout after 5 seconds of inactivity
    timeout++;
    if (timeout > TIMEOUT_5SEC) 
    {
        timeout = 0;
        index = 0;
        blink_freq = 0;
        blink_toggle = 0;
        disp_once = 1;

        // clear entered password
        for (int i = 0; i < PASS_LEN; i++) 
            user_pass[i] = 0;
            
        return TIMEOUT;
    }

    __delay_us(100);
    return CONTINUE;
}