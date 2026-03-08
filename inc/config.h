/******************************************************************************************
* Author         : Labhesh Patil
* Date           : December 18, 2025
* File           : config.h
* Title          : System configuration constants
* Description    : Containing all projects constants
*                 including timing thresholds, log configuration, and UART settings.
*******************************************************************************************/
#ifndef CONFIG_H
#define CONFIG_H

// timing thresholds
#define LONG_PRESS_THRESHOLD    25
#define SHORT_PRESS_THRESHOLD   5
#define BLINK_FREQ_DIVIDER      2500
#define TIMEOUT_5SEC            50000

// Log configuration
#define MAX_LOG_ENTRIES         10
#define LOG_SIZE_BYTES          12
#define EEPROM_LOG_START        0x00

// UART configuration
#define UART_BAUD_RATE          9600

#endif /* CONFIG_H */