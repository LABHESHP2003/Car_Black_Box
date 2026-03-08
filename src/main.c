/**********************************************************************************************
* Author         : Labhesh Patil
* Date           : December 6, 2025
* File           : main.c
* Title          : Main program entry point
* Description    : Initializes all hardware modules and implements the main state machine
*                 that switches between dashboard, login, and menu screens based on
*                 user input and state.
*************************************************************************************************/
#include <xc.h>
#include "main.h"

#pragma config WDTE = OFF
#define _XTAL_FREQ 20000000

// Initialize all modules
void init_config() 
{
    init_adc();
    init_clcd();
    init_digital_keypad();
    init_i2c(100000);
    init_ds1307();
    init_password();
}

void main(void) 
{
    init_config();

    unsigned char key = 0;
    unsigned char operation = MODE_DASHBOARD;

    while (1) 
    {
        key = read_digital_keypad(STATE);

        // enter login mode on SW4 press from dashboard
        if (operation != MODE_MENU && key == SW4) 
        {
            operation = MODE_LOGIN;
        }

        // Modes for different operation
        if (operation == MODE_DASHBOARD) 
        {
            dashboard(key);
        } 
        else if (operation == MODE_LOGIN) 
        {
            operation = login_screen(operation);
        } 
        else if (operation == MODE_MENU) 
        {
            operation = menu(operation);
        } 
        else 
        {
            dashboard(key);
        }
    }
}

// load password from internal EEPROM
void init_password() 
{
    unsigned char i = 0x01;

    // check if password is already stored
    if(eeprom_read(0x00) != 0xAA) 
    {
        // first time store default password
        //first byte reserved to identify if the password is present
        eeprom_write(0x00, 0xAA);

        char default_pass[PASS_LEN] = {'x','y','x','y'};

        for(int k = 0; k < PASS_LEN; k++) 
        {
            correct_pass[k] = default_pass[k];
            eeprom_write(i++, correct_pass[k]);
        }
    }
    else 
    {
        // load existing password from EEPROM
        for(int k = 0; k < PASS_LEN; k++) 
        {
            correct_pass[k] = eeprom_read(i++);
        }
    }
}