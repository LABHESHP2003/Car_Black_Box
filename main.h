/* 
 * File:   main.h
 * Author: labhe
 *
 * Created on December 6, 2025, 9:27 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#include "adc.h"
#include "clcd.h"
#include "dkp.h"
#include "ds1307.h"
#include "i2c.h"
#include "ext_eeprom.h"
#include "password.h"
#include <xc.h>


#pragma config WDTE = OFF
#define _XTAL_FREQ 20000000

#define MAX_ATTEMPTS 3
#define PASS_LEN 4

extern char correct_pass[PASS_LEN];

void dashboard(unsigned char key);
unsigned char login_screen(unsigned char operation);
unsigned char menu(unsigned char operation);

void store_event(unsigned char *time, char *event, char *speed);
void read_event(unsigned char index, char *buffer);
void clear_all_logs(void);
unsigned char get_log_index(void);
unsigned char get_log_count(void);
void view_log_screen(void);

void download_log(void);
void set_time_screen(void);
//void change_password_screen(void);
void init_password(void);

//void menu();
#endif	/* MAIN_H */

