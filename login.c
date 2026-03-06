/*
 * File:   login.c
 * Author: labhesh
 *
 * Created on December 7, 2025, 5:33 PM
 */

#include <xc.h>
#include "main.h"

//#define MAX_ATTEMPTS 3
//#define PASS_LEN 4      // Corrected to match initialization

// Correct password
//char correct_pass[PASS_LEN] = {'x', 'y', 'x', 'y'};
char correct_pass[PASS_LEN];
char user_pass[PASS_LEN];   // User input

unsigned int attempts = MAX_ATTEMPTS;
unsigned int index = 0;
unsigned int blink_freq = 0;
unsigned char blink_toggle = 0;

unsigned long block_counter = 0;
unsigned char is_blocked = 0;

unsigned char key;
unsigned int timeout = 0;   // removed static, optional

unsigned char disp_once = 1;

// --------------------
// Compare password
// --------------------
int compare_pass(const char *p1, const char *p2) {
    for (int i = 0; i < PASS_LEN; i++)
        if (p1[i] != p2[i])
            return 1;   // mismatch
    return 0;           // match
}

// --------------------
// Login screen logic
// --------------------
unsigned char login_screen(unsigned char login_mode) {

    // -------------------- Blocked State --------------------
    if (is_blocked) {
        clcd_print(" LOCKED 2 MIN   ", LINE1(0));
        clcd_print(" TRY LATER      ", LINE2(0));

        // 6 sec x 20 = 2 min
        for (int i = 0; i < 20; i++) {
            __delay_ms(6000);
        }

        // Reset after block
        is_blocked = 0;
        attempts = MAX_ATTEMPTS;
        index = 0;
        timeout = 0;
        disp_once = 1;

        return login_mode;
    }

    // -------------------- Display Login Screen --------------------
    if (disp_once) {
        clcd_clear();
        clcd_print("ENTER PASSWORD  ", LINE1(0));
        clcd_print("                ", LINE2(0));
        disp_once = 0;
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
                return login_mode; // invalid key, ignore

            clcd_putch('*', LINE2(index));
            index++;
            login_mode = 1;

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

            return 2;   // Login successful
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
        login_mode = 0;
        timeout = 0;
        index = 0;
        blink_freq = 0;
        blink_toggle = 0;
        attempts = MAX_ATTEMPTS;
        disp_once = 1;

        // Clear user_pass
        for (int i = 0; i < PASS_LEN; i++) user_pass[i] = 0;

        return 0;
    }

    __delay_us(100);
    return login_mode;
}
