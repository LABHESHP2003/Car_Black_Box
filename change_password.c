/*
 * File:   change_password.c
 * Author: labhesh
 *
 * Created on February 23, 2026, 12:27 PM
 */


#include <xc.h>
#include "main.h"

char user_pass[PASS_LEN]; // User input

unsigned int attempts = MAX_ATTEMPTS;
unsigned int index = 0;
unsigned int blink_freq = 0;
unsigned char blink_toggle = 0;

unsigned long block_counter = 0;
unsigned char is_blocked = 0;

unsigned char key;
unsigned int timeout = 0; // removed static, optional

unsigned char disp_once = 1;

// --------------------
// Login screen logic
// --------------------

int change_password() {

    //old password
    unsigned int ret_old_pass = 0,ret_new_pass = 0;
    while ((ret_old_pass = old_password()) == 0) {
        ;
    }

    if (ret_old_pass == 1) {
        while((ret_new_pass = new_password()) == 0){
            ;
        }
        if(ret_new_pass == 2){
            return 1;
        }
    }
    
    return 1;
    //new password

}

unsigned int old_password() {
    // -------------------- Display Login Screen --------------------
    if (disp_once) {
        clcd_print(" ENTER OLD PASS ", LINE1(0));
        clcd_print("                ", LINE2(0));
        disp_once = 0;
        __delay_ms(1000);
    }

    // -------------------- Cursor Blink --------------------
    if (index < PASS_LEN) {
        if (blink_freq++ >= 2500) {
            blink_freq = 0;
            clcd_putch(blink_toggle ? '_' : 255, LINE2(index));
            blink_toggle ^= 1;
        }
    }

    // -------------------- Read Keypad --------------------
    key = read_digital_keypad(STATE);

    if (key != ALL_RELEASED) {
        timeout = 0;

        if (index < PASS_LEN) {
            if (key == SW4)
                user_pass[index] = 'x';
            else if (key == SW5)
                user_pass[index] = 'y';
            else
                return 0; // invalid key, ignore

            clcd_putch('*', LINE2(index));
            index++;

            blink_toggle = 0;
            blink_freq = 0;

            __delay_ms(200);
        }
    }

    // -------------------- Check Password --------------------
    if (index == PASS_LEN) {
        blink_toggle = 0;

        if (compare_pass(user_pass, correct_pass) == 0) {
            clcd_print("CORRECT PASSWORD", LINE1(0));
            clcd_print("LOGIN SUCCESSFUL", LINE2(0));
            __delay_ms(2000);

            // Clear display
            clcd_print("                ", LINE1(0));
            clcd_print("                ", LINE2(0));

            // Reset login state
            index = 0;
            disp_once = 1;
            timeout = 0;
            blink_freq = 0;
            blink_toggle = 0;
            attempts = MAX_ATTEMPTS;
            for (int i = 0; i < PASS_LEN; i++) user_pass[i] = 0;

            return 1; // Login successful
        }
        else {
            attempts--;
            index = 0;
            disp_once = 1;
            blink_freq = 0;
            blink_toggle = 0;

            clcd_print(" WRONG PASSWORD  ", LINE1(0));
            clcd_print("Attempts Left :  ", LINE2(0));
            clcd_putch(attempts + '0', LINE2(15));

            // Clear user_pass
            for (int i = 0; i < PASS_LEN; i++) user_pass[i] = 0;

            __delay_ms(2000);

            if (attempts == 0)
                is_blocked = 1;
        }
    }

    // -------------------- Timeout after 5 sec --------------------
    timeout++;
    if (timeout > 50000) {
        timeout = 0;
        index = 0;
        blink_freq = 0;
        blink_toggle = 0;
        attempts = MAX_ATTEMPTS;
        disp_once = 1;

        // Clear user_pass
        for (int i = 0; i < PASS_LEN; i++) user_pass[i] = 0;
        return 2;
    }

    __delay_us(100);
    return 0;
}

unsigned int new_password() {

    // -------------------- Display Login Screen --------------------
    if (disp_once) {
        clcd_print(" ENTER NEW PASS ", LINE1(0));
        clcd_print("                ", LINE2(0));
        disp_once = 0;
        __delay_us(1000);
    }

    // -------------------- Cursor Blink --------------------
    if (index < PASS_LEN) {
        if (blink_freq++ >= 2500) {
            blink_freq = 0;
            clcd_putch(blink_toggle ? '_' : 255, LINE2(index));
            blink_toggle ^= 1;
        }
    }

    // -------------------- Read Keypad --------------------
    key = read_digital_keypad(STATE);

    if (key != ALL_RELEASED) {
        timeout = 0;

        if (index < PASS_LEN) {
            if (key == SW4)
                user_pass[index] = 'x';
            else if (key == SW5)
                user_pass[index] = 'y';
            else
                return 0; // invalid key, ignore

            clcd_putch('*', LINE2(index));
            index++;

            blink_toggle = 0;
            blink_freq = 0;

            __delay_ms(200);
        }
    }

    // -------------------- Check Password --------------------
    if (index == PASS_LEN) {
        blink_toggle = 0;

        clcd_print(" PASSWORD CHANGED", LINE1(0));
        clcd_print("   SUCCESSFULLY  ", LINE2(0));
        __delay_ms(2000);

        // Clear display
        clcd_clear();

        eeprom_write(0x00, 0xAA);
        unsigned char i = 0x01;
        for (int k = 0; k < PASS_LEN; k++) {
            eeprom_write(i++, user_pass[k]);
        }
        init_password();


        // Reset login state
        index = 0;
        disp_once = 1;
        timeout = 0;
        blink_freq = 0;
        blink_toggle = 0;
        for (int i = 0; i < PASS_LEN; i++) user_pass[i] = 0;

        return 1; // Login successful
    }

    // -------------------- Timeout after 5 sec --------------------
    timeout++;
    if (timeout > 50000) {
        timeout = 0;
        index = 0;
        blink_freq = 0;
        blink_toggle = 0;
        disp_once = 1;

        // Clear user_pass
        for (int i = 0; i < PASS_LEN; i++) user_pass[i] = 0;
        return 2;
    }

    __delay_us(100);
    return 0;
}