/*
 * File:   view_log.c
 * Author: labhe
 *
 * Created on December 19, 2025, 4:16 PM
 */


#include <xc.h>
#include "uart.h"
#include "main.h"
#pragma config WDTE = OFF
#define _XTAL_FREQ 20000000

#define LINE_LENGTH 16

void init_conf() {
    init_uart(9600);
}

void download_log(void) {
    init_conf();
    unsigned char addr = 0x00;
    unsigned char ch;
    unsigned char log_count = get_log_count();

    clcd_print("Tx Logs UART", LINE1(0));
    clcd_print("Please Wait...", LINE2(0));

    putchar('\r');
    putchar('\n');
    putchar('I');
    putchar('D');
    putchar(' ');
    putchar(' ');
    putchar('T');
    putchar('I');
    putchar('M');
    putchar('E');
    putchar(' ');
    putchar(' ');
    putchar(' ');
    putchar(' ');
    putchar('E');
    putchar('V');
    putchar(' ');
    putchar('S');
    putchar('P');
    putchar('\r');
    putchar('\n');

    for (unsigned char log_no = 0; log_no < log_count; log_no++) {
        // Print log index
        putchar('0' + log_no);
        putchar(' ');

        // Read and print 12 bytes of one log
        for (unsigned char i = 0; i < 12; i++) {
            ch = read_ext_eeprom(addr++);
            putchar(ch);
            if ((i + 1) == 2 || (i + 1) == 4) {
                putchar(':');
            } else if ((i + 1) == 10) {
                putchar(' ');
            }
            __delay_ms(2);
        }

        putchar('\r');
        putchar('\n');
    }

    clcd_print("Done", LINE2(5));
//    clcd_print("                ", LINE1(0));
//    clcd_print("                ", LINE2(0));
    clcd_clear();

    __delay_ms(1000);
}

void change_password_screen(void) {
    clcd_print("Change Password", LINE1(0));
    clcd_print("Not Implemented", LINE2(0));

    __delay_ms(2000);
}

void view_log_screen(void) {
    unsigned char current = 0;
    unsigned char key;
    char log[13]; // 12 bytes + null terminator
    char display[15]; // Formatted string: "HH:MM:SS EV SP"
    unsigned char index;
    unsigned char log_index_local, log_count_local;
    unsigned char cursor[2] = {' ', ' '};

    log_index_local = get_log_index();
    log_count_local = get_log_count();

    if (log_count_local == 0) {
        clcd_print("NO LOG AVAILABLE", LINE1(0));
        clcd_print("                ", LINE2(0));
        __delay_ms(2000);
        return;
    }

    unsigned char sw4_prev = 0;
    int timer = 0;
    while (1) {
        // Circular log index
        index = (log_index_local + 9 - current) % 10;

        // Read log entry
        read_event(index, log);
        log[12] = '\0';

        // Format display (UNCHANGED)
        display[0] = log[0];
        display[1] = log[1];
        display[2] = ':';
        display[3] = log[2];
        display[4] = log[3];
        display[5] = ':';
        display[6] = log[4];
        display[7] = log[5];
        display[8] = ' ';
        display[9] = log[8];
        display[10] = log[9];
        display[11] = ' ';
        display[12] = log[10];
        display[13] = log[11];
        display[14] = '\0';

        /* -------- CURSOR LOGIC (ONLY CHANGE) -------- */
        cursor[0] = ' ';
        cursor[1] = ' ';

        if (current == 0) {
            // Most recent ? can scroll DOWN only
            cursor[0] = 94;
        } else if (current == log_count_local - 1) {
            // Oldest ? can scroll UP only
            cursor[1] = 118;
        } else {
            // Middle ? both directions
            cursor[0] = 94;
            cursor[1] = 118;
        }
        /* -------------------------------------------- */

        // Display (UNCHANGED)
        clcd_putch(cursor[0], LINE1(0));
        clcd_print("  TIME   EV SP", LINE1(2));
        clcd_putch(cursor[1], LINE2(0));
        clcd_print(display, LINE2(2));

        // Key handling (UNCHANGED)
        //key = read_digital_keypad(LEVEL);
        key = read_digital_keypad(STATE);


    if (key == SW4)
    {
        timer++;

        // short press ? scroll once
        if (!sw4_prev && current < log_count_local - 1)
        {
            current++;
        }

        // long press ? exit
        if (timer >= 10)   // ~1 second
        {
            timer = 0;
            clcd_print("                ", LINE1(0));
            clcd_print("                ", LINE2(0));
            return;
        }

        sw4_prev = 1;
    }
    else
    {
        // key released
        timer = 0;
        sw4_prev = 0;

        if (key == SW5 && current > 0)
        {
            current--;
        }
    }

    //__delay_ms(100);
    }
}
