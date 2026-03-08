/***************************************************************************************************
* Author         : Labhesh Patil
* Date           : December 18, 2025
* File           : view_log.c
* Title          : Log viewing and download
* Description    : Provides two functions - view logs on CLCD with scrolling capability,
*                 and download all logs via UART. In view mode, SW4/SW5 scroll through
*                 logs, long press SW4 returns to dashboard. Download mode transmits
*                 logs using UART.
***************************************************************************************************/
#include <xc.h>
#include "uart.h"
#include "main.h"
#include "config.h"

#pragma config WDTE = OFF
#define _XTAL_FREQ 20000000

#define LINE_LENGTH 16

// initialize UART
void init_conf() 
{
    init_uart(UART_BAUD_RATE);
}

// download all logs via UART
void download_log(void) 
{
    init_conf();
    unsigned char addr = EEPROM_LOG_START;
    unsigned char ch;
    unsigned char log_count = get_log_count();

    clcd_print("Tx Logs UART  ", LINE1(0));
    clcd_print("Please Wait...", LINE2(0));

    // send headings
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

    // send each log entry
    for (unsigned char log_no = 0; log_no < log_count; log_no++) 
    {
        // print log ID
        putchar('0' + log_no);
        putchar(' ');

        // read and send LOG_SIZE_BYTES of current log
        for (unsigned char i = 0; i < LOG_SIZE_BYTES; i++) 
        {
            ch = read_ext_eeprom(addr++);
            putchar(ch);
            
            // ':' for readability of time
            if ((i + 1) == 2 || (i + 1) == 4) 
            {
                putchar(':');
            } 
            // space before event
            else if ((i + 1) == 10) 
            {
                putchar(' ');
            }
            __delay_ms(2);
        }

        putchar('\r');
        putchar('\n');
    }

    clcd_print("Done", LINE2(5));
    clcd_clear();

    __delay_ms(2000);
}

// view logs
void view_log_screen(void) 
{
    // current position from newest
    unsigned char current = 0;
    // for long press detection
    unsigned char key_level;
    // for short press navigation
    unsigned char key_state;
    //Log 
    char log[LOG_SIZE_BYTES + 1];
    char display[15];
    unsigned char index;
    unsigned char log_index_local, log_count_local;
    unsigned char cursor[2] = {' ', ' '};
    
    // long press detection variables
    unsigned int sw4_press_time = 0;
    unsigned char sw4_pressed = 0;

    log_index_local = get_log_index();
    log_count_local = get_log_count();

    // No logs available
    if (log_count_local == 0) 
    {
        clcd_print("NO LOG AVAILABLE", LINE1(0));
        clcd_print("                ", LINE2(0));
        __delay_ms(2000);
        return;
    }

    while (1) 
    {
        // calculate actual index in circular buffer newest first
        index = (log_index_local + MAX_LOG_ENTRIES - 1 - current) % MAX_LOG_ENTRIES;

        // read log entry from EEPROM
        read_event(index, log);
        log[LOG_SIZE_BYTES] = '\0';

        // log format -> (HH:MM:SS EV SP)
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

        // cursor indication
        cursor[0] = ' ';
        cursor[1] = ' ';

        if (current == 0) 
        {
            // up arrow using '^'
            cursor[0] = 94;
        } 
        else if (current == log_count_local - 1) 
        {
            // down arrow using 'v'
            cursor[1] = 118;
        } 
        else 
        {
            // both directions
            cursor[0] = 94;
            cursor[1] = 118;
        }

        // display log with cursor indicator
        clcd_putch(cursor[0], LINE1(0));
        clcd_print("  TIME   EV SP", LINE1(2));
        clcd_putch(cursor[1], LINE2(0));
        clcd_print(display, LINE2(2));

        // read keys in both modes
        // long press detection
        key_level = read_digital_keypad(LEVEL);
        // edge trigger for navigation
        key_state = read_digital_keypad(STATE);

        //SW4 (UP/EXIT)
        if (key_level == SW4)
        {
            // SW4 is being held down
            if (!sw4_pressed)
            {
                // reset SW4 press time if edge trigger
                sw4_pressed = 1;
                sw4_press_time = 0;
            }
            else
            {
                // long press increment timer
                sw4_press_time++;
                
                // if long press detected then exit
                if (sw4_press_time >= LONG_PRESS_THRESHOLD)
                {
                    clcd_clear();
                    return;
                }
            }
        }
        else
        {
            if (sw4_pressed)
            {
                // short press -> scroll up
                if (sw4_press_time < LONG_PRESS_THRESHOLD && current < log_count_local - 1)
                {
                    current++;
                }
                sw4_pressed = 0;
                sw4_press_time = 0;
            }
        }

        //SW5(DOWN)
        if (key_state == SW5 && current > 0)
        {
            current--;
            __delay_ms(200);
        }

        __delay_ms(10);
    }
}