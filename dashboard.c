/*
 * File:   dashboard.c
 * Author: labhesh
 *
 * Created on December 7, 2025, 4:47 PM
 */


#include <xc.h>
#include "main.h"

unsigned char clock_reg[3];
unsigned char pos = 0;
unsigned short adc_value;
unsigned char speed[3] = {0};
char *event[] = {"ON", "GN", "G1", "G2", "G3", "G4", "G5", "GR", "C "};

void dashboard(unsigned char key) {
    //Dashboard
    clcd_print("  TIME    EV  SP", LINE1(0));

    //Display Time
    get_time(clock_reg);
    display_time(clock_reg,0);

    //display collision
    if (key == SW1) {
        pos = 8;
    }//increment gear
    else if (key == SW2) {
        if (pos < 7) {
            ++pos;
        }
    }//decrement gear
    else if (key == SW3) {
        if (pos > 1) {
            --pos;
        }
    }

    //Display Event
    clcd_print(event[pos], LINE2(10));

    //Display Speed
    adc_value = read_adc();
    adc_value = (adc_value) / 10.23;

    speed[1] = (adc_value % 10) + 48;
    speed[0] = (adc_value / 10) + 48;

    clcd_print(speed, LINE2(14));

    if (key == SW1 || key == SW2 || key == SW3) {
        store_event(clock_reg, event[pos], speed);
    }

}
