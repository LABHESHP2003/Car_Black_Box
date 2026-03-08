/*********************************************************************************************
* Author         : Labhesh Patil
* Date           : December 12, 2025
* File           : menu.c
* Title          : Menu system handler
* Description    : Implements a 5-item menu system with cursor navigation.
*                 Short press SW4/SW5 to move cursor, long press to select.
*                 Long press SW4 returns to dashboard. Handles menu selection
*                 and calls appropriate functions.
*************************************************************************************************/
#include <xc.h>
#include "main.h"
#include "config.h"

// menu navigation variables
unsigned int menu_selector = 0;
unsigned char cursor = 1;
unsigned char key;

// Menu options display strings
char *menu_e[] = {" View log      ", " Clear log     ", " Download log  ", 
                  " Set Time      ", " Change Passwd "};

// key press timing for long press detection
unsigned int sw4_time = 0, sw5_time = 0;
unsigned char reset1 = 0, reset2 = 0;

unsigned char menu(unsigned char operation) 
{
    // display cursor at current position
    if (cursor == 1) 
    {
        clcd_putch('*', LINE1(0));
        clcd_putch(' ', LINE2(0));
    } 
    else 
    {
        clcd_putch('*', LINE2(0));
        clcd_putch(' ', LINE1(0));
    }

    // display two menu i.e current and the next menu
    clcd_print(menu_e[menu_selector], LINE1(1));
    clcd_print(menu_e[menu_selector + 1], LINE2(1));

    // Read key
    key = read_digital_keypad(LEVEL);

    //SW4 (UP / BACK)
    if (key == SW4) 
    {
        sw4_time++;

        // long press -> Return to dashboard
        if (sw4_time > LONG_PRESS_THRESHOLD && reset1 == 0) 
        {   
            reset1 = 1;
            operation = MODE_DASHBOARD;
            clcd_clear();
            return operation;
        }
    } 
    else 
    {
        // short press -> Move cursor up
        if (sw4_time > 0 && sw4_time <= SHORT_PRESS_THRESHOLD && reset1 == 0) 
        {
            if (menu_selector > 0) 
            {
                if (menu_selector == 3 && cursor == 2) 
                    cursor = 1;
                else 
                    menu_selector--;
            }
        }
        sw4_time = 0;
        reset1 = 0;
    }

    //SW5 (DOWN / SELECT)
    if (key == SW5)
    {
        sw5_time++;
        
        // long press -> select current menu
        if (sw5_time > LONG_PRESS_THRESHOLD && reset2 == 0)
        {
            reset2 = 1;

            unsigned char selected_index = menu_selector + (cursor - 1);

            switch (selected_index)
            {
                // View log
                case 0:
                    view_log_screen();
                    return MODE_DASHBOARD;
                    break;
                // Clear log
                case 1:
                    clear_all_logs();
                    clcd_print("LOGS CLEARED    ", LINE1(0));
                    clcd_print("                ", LINE2(0));
                    __delay_ms(1500);
                    break;
                // Download log via UART
                case 2:
                    download_log();
                    break;
                // Set time
                case 3:
                    set_time_screen();
                    break;
                // Change password
                case 4:
                    change_password();
                    break;
            }
        }
    }
    else
    {
        // short press-> move cursor down
        if (sw5_time > 0 && sw5_time <= SHORT_PRESS_THRESHOLD && reset2 == 0)
        {
            if (menu_selector <= 3)
            {
                if (menu_selector == 3)
                    cursor = 2;
                else
                    menu_selector++;
            }
        }
        sw5_time = 0;
        reset2 = 0;
    }

    return MODE_MENU;
}