/*
 * File:   main.c
 * Author: labhesh
 *
 * Created on December 6, 2025, 9:29 PM
 */


#include <xc.h>
#include "main.h"

#pragma config WDTE = OFF
#define _XTAL_FREQ 20000000

void init_config() {
    init_adc();
    init_clcd();
    init_digital_keypad();
    init_i2c(100000);
    init_ds1307();
    init_password();
}

void main(void) {
    init_config();

    unsigned char key = 0;
    unsigned char operation = 0;
    //unsigned int timeout;

    while (1) {
        key = read_digital_keypad(STATE);

        /* Temporary SW4 login mode (~2 sec) */
        if (operation != 2  && key == SW4) {
            operation = 1; // enter login screen
            //timeout = 0;    // reset timeout
        }

        if (operation == 0) {
            dashboard(key); // normal dashboard
        } else if (operation == 1) {
            operation = login_screen(operation); // login screen
        } else if (operation == 2) {
            operation = menu(operation);//menu
        } else {
            dashboard(key); // normal dashboard
        }
    }
}

void init_password() {
    unsigned char i = 0x01;

    if(eeprom_read(0x00) != 0xAA) {

        eeprom_write(0x00, 0xAA);

        char default_pass[PASS_LEN] = {'x','y','x','y'};

        for(int k = 0; k < PASS_LEN; k++) {
            correct_pass[k] = default_pass[k];
            eeprom_write(i++, correct_pass[k]);
        }
    }
    else {
        for(int k = 0; k < PASS_LEN; k++) {
            correct_pass[k] = eeprom_read(i++);
        }
    }
}