/*
 * File:   menu.c
 * Author: labhe
 *
 * Created on December 12, 2025, 4:19 PM
 */


#include <xc.h>
#include "main.h"

//unsigned char view_log = 0, clear_log = 0, download_log = 0, set_time = 0, change_password = 0;
unsigned int menu_selector = 0;
unsigned char cursor = 1;
unsigned char key;

char *menu_e[] = {" View log      ", " Clear log     ", " Download log  ", " Set Time      ", " Change Passwd "};

unsigned int SW4_time = 0, SW5_time = 0;
unsigned char reset1 = 0, reset2 = 0;


unsigned char menu(unsigned char operation) {

    // Display cursor
    if (cursor == 1) {
        clcd_putch('*', LINE1(0));
        clcd_putch(' ', LINE2(0));
    } else {
        clcd_putch('*', LINE2(0));
        clcd_putch(' ', LINE1(0));
    }

    // Display menu items
    clcd_print(menu_e[menu_selector], LINE1(1));
    clcd_print(menu_e[menu_selector + 1], LINE2(1));

    // Read key continuously
    key = read_digital_keypad(LEVEL);

    /* -------------------- SW4 (UP) -------------------- */
    if (key == SW4) {
        SW4_time++;

        // LONG PRESS: Select option
        if (SW4_time > 25 && reset1 == 0) {   
            reset1 = 1;
            
            operation = 0;
            clcd_clear();
            return operation;
        }
    } 
    else {
        // SHORT PRESS: Move up
        //if (SW4_time > 0 && SW4_time <= 25 && reset1 == 0) {
        if (SW4_time > 0 && SW4_time <= 5 && reset1 == 0) {
            if (menu_selector > 0) {
                if (menu_selector == 3 && cursor == 2) cursor = 1;
                else menu_selector--;
            }
        }
        SW4_time = 0;
        reset1 = 0;
    }

    /* -------------------- SW5 (DOWN) -------------------- */
    /* -------------------- SW5 (DOWN / SELECT) -------------------- */
if (key == SW5)
{
    SW5_time++;

    // LONG PRESS: Select option
    if (SW5_time > 25 && reset2 == 0)
    {
        reset2 = 1;

        unsigned char selected_index = menu_selector + (cursor - 1);

        switch (selected_index)
        {
            case 0:
                view_log_screen();
                return 0;
                break;

            case 1:
                clear_all_logs();
                clcd_print("LOGS CLEARED    ", LINE1(0));
                clcd_print("                ", LINE2(0));
                __delay_ms(1500);
                break;

            case 2:
                download_log();
                break;

            case 3:
                set_time_screen();
                break;

            case 4:
                change_password();
                break;

            default:
                break;
        }
    }
}
else
{
    // SHORT PRESS: Move down
    //if (SW5_time > 0 && SW5_time <= 25 && reset2 == 0)
    if (SW5_time > 0 && SW5_time <= 5 && reset2 == 0)
    {
        if (menu_selector <= 3)
        {
            if (menu_selector == 3)
                cursor = 2;
            else
                menu_selector++;
        }
    }
    SW5_time = 0;
    reset2 = 0;
}


    return 2; // stay in menu
}
