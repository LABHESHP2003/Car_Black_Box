/***********************************************************************************************
* Author         : Labhesh Patil
* Date           : December 13, 2025
* File           : log.c
* Title          : Event logging manager
* Description    : Manages circular buffer of 10 log entries in external EEPROM.
*                 Each log entry stores 8 bytes time + 2 bytes event + 2 bytes speed.
*                 Provides functions to read, write, and clear logs.
***************************************************************************************************/

#include "main.h"
#include "config.h"

// Log management variables
static unsigned char log_index = 0;
static unsigned char log_count = 0;

// get current log index
unsigned char get_log_index(void)
{
    return log_index;
}

// get total number of logs stored
unsigned char get_log_count(void)
{
    return log_count;
}

// clear all logs from memory
void clear_all_logs(void)
{
    log_index = 0;
    log_count = 0;
}

// read one log entry from EEPROM i.e 12 bytes of data
void read_event(unsigned char index, char *log)
{
    unsigned char addr = EEPROM_LOG_START + (index * LOG_SIZE_BYTES);

    for (unsigned char i = 0; i < LOG_SIZE_BYTES; i++)
    {
        log[i] = read_ext_eeprom(addr++);
        __delay_ms(5);
    }

    log[LOG_SIZE_BYTES] = '\0';
}

// Store event in EEPROM (time + event + speed)
void store_event(unsigned char *clock_reg, char *event, char *speed)
{
    static unsigned char addr;
    unsigned char i;
    char time[8];

    // convert BCD time from DS1307 to ASCII "HHMMSS"
    time[0] = ((clock_reg[0] >> 4) & 0x03) + '0';    // hours tens
    time[1] = (clock_reg[0] & 0x0F) + '0';           // hours units
    time[2] = ((clock_reg[1] >> 4) & 0x07) + '0';    // minutes tens
    time[3] = (clock_reg[1] & 0x0F) + '0';           // minutes units
    time[4] = ((clock_reg[2] >> 4) & 0x07) + '0';    // seconds tens
    time[5] = (clock_reg[2] & 0x0F) + '0';           // seconds units
    time[6] = ' ';                        // padding
    time[7] = ' ';

    // calculate EEPROM address for this log
    addr = EEPROM_LOG_START + (log_index * LOG_SIZE_BYTES);

    // write time to EEPROM i.e writing 8 bytes of data
    for (i = 0; i < 8; i++)
    {
        write_ext_eeprom(addr++, time[i]);
        __delay_ms(5);
    }

    // write event i.e writing 2 bytes
    write_ext_eeprom(addr++, event[0]); 
    __delay_ms(5);
    write_ext_eeprom(addr++, event[1]); 
    __delay_ms(5);

    // write speed i.e writing 2 bytes
    write_ext_eeprom(addr++, speed[0]); 
    __delay_ms(5);
    write_ext_eeprom(addr++, speed[1]); 
    __delay_ms(5);

    log_index++;
    
    // circular buffer log storage
    if (log_index == MAX_LOG_ENTRIES)
        log_index = 0;

    if (log_count < MAX_LOG_ENTRIES)
        log_count++;
}