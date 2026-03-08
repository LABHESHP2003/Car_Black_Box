/*****************************************************************************************
* Author         : Labhesh Patil
* Date           : December 6, 2025
* File           : main.h
* Title          : Main header file
* Description    : Contains all function declarations, macro definitions, and includes
*                 for the Car Black Box project. Defines operation modes and return
*                 status macros used throughout the project.
************************************************************************************************/
#ifndef MAIN_H
#define	MAIN_H

#include "adc.h"
#include "clcd.h"
#include "dkp.h"
#include "ds1307.h"
#include "i2c.h"
#include "ext_eeprom.h"
#include "password.h"
#include "config.h"
#include <xc.h>

#pragma config WDTE = OFF
#define _XTAL_FREQ 20000000

// login configs
#define MAX_ATTEMPTS        3
#define PASS_LEN            4

// operation modes
#define MODE_DASHBOARD      0
#define MODE_LOGIN          1
#define MODE_MENU           2

// return status macros
#define SUCCESS             1
#define FAILURE             0
#define TIMEOUT             2
#define EXIT                2
#define CONTINUE            0
#define BLOCKED             1

extern char correct_pass[PASS_LEN];

// function declaration
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
void init_password(void);

#endif	/* MAIN_H */