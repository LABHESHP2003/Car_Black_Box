/*
 * File:   log.c
 * Author: labhe
 *
 * Created on December 18, 2025, 5:30 PM
 */

#include "main.h"

/* ONLY THIS FILE KNOWS THESE */
static unsigned char log_index = 0;
static unsigned char log_count = 0;

/* Getter functions */
unsigned char get_log_index(void)
{
    return log_index;
}

unsigned char get_log_count(void)
{
    return log_count;
}

/* Clear logs */
void clear_all_logs(void)
{
    log_index = 0;
    log_count = 0;
}


void read_event(unsigned char index, char *log)
{
    unsigned char addr = index * 12;

    for (unsigned char i = 0; i < 12; i++)
    {
        log[i] = read_ext_eeprom(addr++);
        __delay_ms(5);
    }

    log[12] = '\0';
}

// Store an event in EEPROM

void store_event(unsigned char *clock_reg, char *event, char *speed)
{
    static unsigned char addr;
    unsigned char i;
    char time[8]; // 8 bytes to store HHMMSS + 2 spaces

    // Convert BCD from DS1307 to ASCII "HHMMSS"
    time[0] = ((clock_reg[0] >> 4) & 0x03) + '0'; // Hours tens
    time[1] = (clock_reg[0] & 0x0F) + '0';        // Hours units

    time[2] = ((clock_reg[1] >> 4) & 0x07) + '0'; // Minutes tens
    time[3] = (clock_reg[1] & 0x0F) + '0';        // Minutes units

    time[4] = ((clock_reg[2] >> 4) & 0x07) + '0'; // Seconds tens
    time[5] = (clock_reg[2] & 0x0F) + '0';        // Seconds units

    time[6] = ' '; // Padding
    time[7] = ' ';

    addr = log_index * 12; // Each log entry is 12 bytes

    // Write time to EEPROM
    for (i = 0; i < 8; i++)
    {
        write_ext_eeprom(addr++, time[i]);
        __delay_ms(5);
    }

    // Write event (2 bytes)
    write_ext_eeprom(addr++, event[0]); __delay_ms(5);
    write_ext_eeprom(addr++, event[1]); __delay_ms(5);

    // Write speed (2 bytes)
    write_ext_eeprom(addr++, speed[0]); __delay_ms(5);
    write_ext_eeprom(addr++, speed[1]); __delay_ms(5);

    // Update log index and count
    log_index++; 
    if (addr == 120){
        addr = 0;
    }
    if (log_index == 10)
        log_index = 0;

    if (log_count < 10)
        log_count++;
}
