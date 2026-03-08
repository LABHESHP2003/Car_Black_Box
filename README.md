# Car Black Box - PIC16F877A (PIC Genuine Board)

<p align="center">
  <img src="https://img.shields.io/badge/MCU-PIC16F877A-blue.svg">
  <img src="https://img.shields.io/badge/Language-C-blue.svg">
  <img src="https://img.shields.io/badge/Interface-I2C-orange.svg">
  <img src="https://img.shields.io/badge/RTC-DS1307-green.svg">
  <img src="https://img.shields.io/badge/Status-Stable-brightgreen.svg">
</p>

## 📋 Quick Navigation
- [Overview](#-overview)
- [Features](#-features)
- [Hardware Setup](#-hardware-setup)
- [Pin Configuration](#-pin-configuration)
- [Software Installation](#-software-installation)
- [User Manual](#-user-manual)
- [File Structure](#-file-structure)
- [Configuration](#-configuration)
- [Troubleshooting](#-troubleshooting)
- [Downloads](#-downloads)

---

## 📋 Overview

A professional **Car Black Box system** implemented on **PIC16F877A microcontroller** (PIC Genuine Board). This project records vehicle events with timestamps and stores them in external EEPROM, similar to an aircraft black box. Features include password protection, real-time clock, gear tracking, speed monitoring, and collision detection.

**Working Principle:**
- Continuously monitors gear position via keypad (SW1-SW3)
- Reads speed from potentiometer (0-100 km/h)
- Tracks time using DS1307 RTC
- Automatically logs events to external EEPROM
- Provides password-protected menu system for log access

---

## 🎯 Features

### 🖥️ Real-time Dashboard
| Feature | Description |
|---------|-------------|
| **Current Time** | HH:MM:SS format from DS1307 RTC |
| **Gear Position** | ON, GN, G1-G5, GR (Reverse), C (Collision) |
| **Speed Display** | 0-100 km/h via potentiometer (ADC) |
| **Collision Event** | Special marker triggered by SW1 |

### 🔐 Secure Access System
| Feature | Description |
|---------|-------------|
| **Password Protection** | 4-digit using 'x' (SW4) and 'y' (SW5) |
| **Attempt Limit** | 3 attempts then 2-minute lockout |
| **Password Change** | Authorized users can update password |
| **Storage** | Password stored in internal EEPROM |
| **Auto Timeout** | Returns to dashboard after 5s inactivity |

### 📝 Event Logging System
| Feature | Description |
|---------|-------------|
| **Automatic Logging** | Events recorded on any key press |
| **Circular Buffer** | Stores last 10 events in external EEPROM |
| **Log Format** | HH:MM:SS + Event (2 chars) + Speed (2 digits) |
| **Non-volatile** | Data retained after power loss |

### 📊 Log Management
| Feature | Description |
|---------|-------------|
| **View Logs** | Scroll through recorded events on LCD |
| **Clear Logs** | Erase all stored events |
| **Download Logs** | Transmit via UART (9600 baud) |
| **Output Format** | CSV-style for easy parsing |

### ⏰ Time Management
| Feature | Description |
|---------|-------------|
| **Set Time** | Adjust RTC time with field selection |
| **Visual Feedback** | Blinking cursor for current field |
| **Increment/Decrement** | SW5 increase, SW4 decrease |
| **Save/Cancel** | Long press SW5 save, SW4 cancel |

---

## 🛠️ Hardware Setup

### Components Required

| Component | Specification | Quantity | Purpose |
|-----------|--------------|----------|---------|
| **PIC Genuine Board** | PIC16F877A | 1 | Main controller (onboard) |
| **LCD Module** | 16x2 (HD44780) | 1 | Display interface |
| **RTC Module** | DS1307 | 1 | Real-time clock |
| **EEPROM** | 24C04/24C08 | 1 | External log storage |
| **Keypad** | Tactile switches | 6 | User input (SW1-SW6) |
| **Potentiometer** | 10kΩ linear | 1 | Speed simulation |
| **Potentiometer** | 10kΩ | 1 | LCD contrast |
| **Crystal** | 32.768 kHz | 1 | RTC clock |
| **Resistors** | 10kΩ | 8 | Pull-up resistors |
| **Capacitors** | 22pF | 2 | Crystal loading |
| **Battery** | CR2032 | 1 | RTC backup |

### Connection Diagram

```
                    +5V
                     |
                    [10kΩ] Pull-ups (x8)
                     |
PIC16F877A    RB0──SW1──GND    (Collision)
              RB1──SW2──GND    (Gear Up)
              RB2──SW3──GND    (Gear Down)
              RB3──SW4──GND    (Menu Up / 'x')
              RB4──SW5──GND    (Menu Down / 'y')
              RB5──SW6──GND    (Reserved)

              RC3───┬───SCL (DS1307) ───SCL (24C04)
              RC4───┴───SDA (DS1307) ───SDA (24C04)
                    │
                   [10kΩ] Pull-ups to +5V

              RA0───[10kΩ Pot]───GND    (Speed Sensor)

              RD0-RD7───LCD D0-D7
              RE1───────LCD EN
              RE2───────LCD RS
              LCD RW────GND
              LCD V0────[10kΩ Pot]───GND (Contrast)

              RC6───────TX (to USB-TTL/MAX232)
              RC7───────RX (to USB-TTL/MAX232)

              OSC1/OSC2──20MHz Crystal (onboard)
              32.768kHz Crystal on DS1307 with 22pF caps
```

---

## 🔌 Pin Configuration

### PIC16F877A Pin Assignment

| Port Pin | Function | Connected To | Description |
|----------|----------|--------------|-------------|
| **RA0** | AN0 | 10kΩ Potentiometer | Speed input (0-5V) |
| **RB0** | Digital Input | SW1 | Collision trigger |
| **RB1** | Digital Input | SW2 | Gear up |
| **RB2** | Digital Input | SW3 | Gear down |
| **RB3** | Digital Input | SW4 | Menu up / 'x' |
| **RB4** | Digital Input | SW5 | Menu down / 'y' |
| **RB5** | Digital Input | SW6 | Reserved |
| **RC3** | SCL (I2C) | DS1307 + 24C04 SCL | I2C Clock |
| **RC4** | SDA (I2C) | DS1307 + 24C04 SDA | I2C Data |
| **RC6** | TX (UART) | USB-TTL/MAX232 | Transmit to PC |
| **RC7** | RX (UART) | USB-TTL/MAX232 | Receive from PC |
| **RD0-7** | Output | LCD D0-D7 | LCD data |
| **RE1** | Output | LCD EN | LCD enable |
| **RE2** | Output | LCD RS | LCD register select |

### I2C Device Addresses

| Device | Address | Notes |
|--------|---------|-------|
| **DS1307** | 0xD0 (write) / 0xD1 (read) | RTC |
| **24C04** | 0xA0 (write) / 0xA1 (read) | EEPROM (A0-A2 = GND) |

---

## 💻 Software Installation

### Prerequisites

| Software | Version | Download Link |
|----------|---------|---------------|
| **MPLAB X IDE** | v5.40+ | [Microchip Site](https://www.microchip.com/mplab/mplab-x-ide) |
| **XC8 Compiler** | v2.32+ | [Microchip Site](https://www.microchip.com/mplab/compilers) |
| **PICkit3/4** | - | Programmer/Debugger |
| **Terminal** | Any | PuTTY, TeraTerm, Arduino Serial Monitor |

### Step-by-Step Installation

#### 1️⃣ Install Software
```bash
# Download and install MPLAB X IDE
# Download and install XC8 Compiler
# Ensure compiler path is configured in MPLAB
```

#### 2️⃣ Create Project in MPLAB X
```
File → New Project
  → Microchip Embedded → Standalone Project
  → Device: PIC16F877A
  → Tool: PICkit3 (or your programmer)
  → Compiler: XC8
  → Project Name: Car_Black_Box
  → Finish
```

#### 3️⃣ Add Files to Project
```
Source Files:   Right-click → Add Existing Item → select all .c files from src/
Header Files:   Right-click → Add Existing Item → select all .h files from inc/
Drivers:        Right-click → Add Existing Item → select all .c files from drivers/
```

#### 4️⃣ Configure Compiler Settings
```
Right-click Project → Properties → XC8 Compiler → Additional options:
  -std=c99
  -O1
```

#### 5️⃣ Build and Program
```
Clean and Build (Shift+F11) → Verify "BUILD SUCCESSFUL"
Connect Programmer → Make and Program Device (F6)
```

---

## 📖 User Manual

### First Power-On Sequence

| Step | Action | Expected Result |
|------|--------|-----------------|
| 1 | Apply 5V power | LCD backlight ON |
| 2 | Wait 2 seconds | "ENTER PASSWORD" on LCD |
| 3 | Enter **xyxy** using SW4(x) and SW5(y) | Characters show as '*' |
| 4 | After 4th character | "LOGIN SUCCESSFUL" then Dashboard |

### Dashboard Mode

**Display Format:**
```
  TIME    EV  SP
12:34:56  G3  45
```

| Key | Action | Description |
|-----|--------|-------------|
| **SW1** | Collision | Records collision event (C ) |
| **SW2** | Gear Up | G1→G2→G3→G4→G5 |
| **SW3** | Gear Down | G5→G4→G3→G2→G1→GN |
| **SW4** | Hold 2 sec | Enter login screen |

**Gear States:**
| Display | Meaning |
|---------|---------|
| **ON** | Ignition ON |
| **GN** | Neutral |
| **G1-G5** | Gears 1-5 |
| **GR** | Reverse |
| **C** | Collision event |

### Menu System

**To Enter Menu:**
1. From dashboard, hold SW4 (~2 seconds)
2. Enter password
3. Menu appears

**Menu Display:**
```
* View log      
  Clear log     
```

| Key | Action |
|-----|--------|
| **SW4 Short** | Move cursor up |
| **SW4 Long** | Return to dashboard |
| **SW5 Short** | Move cursor down |
| **SW5 Long** | Select current option |

**Menu Options:**
| Option | Function |
|--------|----------|
| **View log** | Browse recorded events |
| **Clear log** | Delete all stored logs |
| **Download log** | Transmit logs via UART |
| **Set Time** | Adjust RTC clock |
| **Change Passwd** | Update access code |

### View Log Mode

**Display:**
```
LOG 1/5  TIME   EV SP
12:34:56  G3  45
```

| Key | Action |
|-----|--------|
| **SW4 Short** | Next log (newer) |
| **SW5 Short** | Previous log (older) |
| **SW4 Long** | Exit to dashboard |

### Set Time Mode

**Display:**
```
SET TIME
12:34:56
```

| Key | Action |
|-----|--------|
| **SW4 Short** | Decrease current field |
| **SW4 Long** | Cancel, return to menu |
| **SW5 Short** | Increase current field |
| **SW5 Long** | Next field / Save |

*Fields cycle: Hours → Minutes → Seconds → Save*

### Download Logs via UART

1. **Connect Hardware:**
   ```
   PIC RC6 (TX) ──── USB-TTL RX
   PIC RC7 (RX) ──── USB-TTL TX
   PIC GND ───────── USB-TTL GND
   ```

2. **Open Terminal:**
   - Baud Rate: **9600**
   - Data Bits: **8**
   - Parity: **None**
   - Stop Bits: **1**

3. **Select "Download log" from menu**

4. **Terminal Output:**
   ```
   ID  TIME    EV SP
   0  12:34:56 G3 45
   1  12:35:10 G4 60
   2  12:35:45 G5 78
   3  12:36:20 C  45
   ```

---

## 📁 File Structure

```
Car_Black_Box/
│
├── inc/                           # Header files
│   ├── main.h                     # Main header, operation modes
│   ├── config.h                   # System configuration
│   ├── adc.h                      # ADC driver header
│   ├── clcd.h                     # LCD driver header
│   ├── dkp.h                      # Keypad driver header
│   ├── ds1307.h                   # RTC driver header
│   ├── ext_eeprom.h               # EEPROM driver header
│   ├── i2c.h                      # I2C driver header
│   ├── password.h                 # Password management header
│   └── uart.h                     # UART driver header
│
├── src/                           # Application source files
│   ├── main.c                     # Main program, initialization
│   ├── login.c                    # Login screen handler
│   ├── dashboard.c                # Main dashboard display
│   ├── menu.c                     # Menu system handler
│   ├── log.c                      # Event logging manager
│   ├── view_log.c                 # Log viewing and download
│   ├── set_time.c                 # RTC time setting
│   └── change_password.c          # Password change handler
│
├── drivers/                       # Hardware driver source files
│   ├── adc.c                      # ADC module driver
│   ├── clcd.c                     # LCD driver
│   ├── dkp.c                      # Digital keypad driver
│   ├── ds1307.c                   # RTC driver
│   ├── ext_eeprom.c               # External EEPROM driver
│   ├── i2c.c                      # I2C master driver
│   └── uart.c                     # UART driver
│
└── README.md                      # This file
```

---

## ⚙️ Configuration

### config.h

```c
#ifndef CONFIG_H
#define CONFIG_H

// Timing thresholds (20ms loop)
#define LONG_PRESS_THRESHOLD    25      // ~500ms
#define SHORT_PRESS_THRESHOLD   5       // ~100ms
#define BLINK_FREQ_DIVIDER      2500    // Cursor blink rate
#define TIMEOUT_5SEC            50000   // 5-second timeout

// Log configuration
#define MAX_LOG_ENTRIES         10      // Circular buffer size
#define LOG_SIZE_BYTES          12      // Bytes per log entry
#define EEPROM_LOG_START        0x00    // Start address

// UART configuration
#define UART_BAUD_RATE          9600    // Communication speed

#endif
```

### main.h

```c
#ifndef MAIN_H
#define MAIN_H

// Login configuration
#define MAX_ATTEMPTS        3
#define PASS_LEN            4

// Operation modes
#define MODE_DASHBOARD      0
#define MODE_LOGIN          1
#define MODE_MENU           2

// Return status macros
#define SUCCESS             1
#define FAILURE             0
#define TIMEOUT             2
#define CONTINUE            0

#endif
```

---

## 🔍 Troubleshooting

### Common Issues & Solutions

| Problem | Likely Cause | Solution |
|---------|--------------|----------|
| **No LCD display** | Contrast wrong | Adjust 10kΩ pot on V0 |
| **LCD garbled** | Power issue | Add 10µF capacitor near PIC |
| **No key response** | Missing pull-ups | Add 10kΩ to RB0-RB5 |
| **RTC not working** | Crystal issue | Check 32.768 kHz crystal & caps |
| **RTC losing time** | Dead battery | Replace CR2032 |
| **I2C not responding** | No pull-ups | Add 10kΩ to RC3/RC4 |
| **EEPROM write fails** | WP pin high | Connect WP to GND |
| **No UART output** | Baud rate mismatch | Verify 9600 setting |
| **Cannot enter menu** | Wrong password | Default: xyxy |
| **Logs not saving** | EEPROM address | Check A0-A2 = GND |

### Quick Test Procedure

| Step | Action | Expected Result |
|------|--------|-----------------|
| 1 | Power on | "ENTER PASSWORD" |
| 2 | Enter "xyxy" | Dashboard appears |
| 3 | Press SW2/SW3 | Gear changes |
| 4 | Turn potentiometer | Speed changes (0-100) |
| 5 | Press SW1 | Shows "C " |
| 6 | Hold SW4 | Password prompt |
| 7 | Enter password | Menu appears |

---

## 📊 Log Format

### Memory Map (External EEPROM)

| Address Range | Content |
|--------------|---------|
| 0x00-0x0B | Log Entry 0 (12 bytes) |
| 0x0C-0x17 | Log Entry 1 |
| 0x18-0x23 | Log Entry 2 |
| ... | ... |
| 0x6C-0x77 | Log Entry 9 |

### Single Log Entry Structure

| Bytes | Content | Format |
|-------|---------|--------|
| 0-1 | Hours | ASCII (HH) |
| 2-3 | Minutes | ASCII (MM) |
| 4-5 | Seconds | ASCII (SS) |
| 6-7 | Padding | Spaces |
| 8-9 | Event | ASCII (2 chars) |
| 10-11 | Speed | ASCII (2 digits) |

---

## 📋 Technical Specifications

| Parameter | Value |
|-----------|-------|
| **Microcontroller** | PIC16F877A |
| **Clock Frequency** | 20 MHz |
| **Supply Voltage** | 5V DC |
| **Current Consumption** | ~50 mA |
| **LCD** | 16x2 Character |
| **RTC** | DS1307 with battery backup |
| **EEPROM** | 24C04 (512 bytes) |
| **Log Capacity** | 10 events |
| **Password Length** | 4 characters |
| **Max Attempts** | 3 |
| **Lockout Time** | 2 minutes |
| **UART Baud Rate** | 9600 |
| **Speed Range** | 0-100 km/h |
| **Gear Positions** | ON, GN, G1-G5, GR, C |

---

## 📥 Downloads

### Complete File Contents

<details>
<summary><b>📁 inc/main.h</b></summary>

```c
#ifndef MAIN_H
#define MAIN_H

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

// Login configuration
#define MAX_ATTEMPTS        3
#define PASS_LEN            4

// Operation modes
#define MODE_DASHBOARD      0
#define MODE_LOGIN          1
#define MODE_MENU           2

// Return status macros
#define SUCCESS             1
#define FAILURE             0
#define TIMEOUT             2
#define EXIT                2
#define CONTINUE            0
#define BLOCKED             1

extern char correct_pass[PASS_LEN];

// Function declarations
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

#endif
```
</details>

<details>
<summary><b>📁 inc/config.h</b></summary>

```c
#ifndef CONFIG_H
#define CONFIG_H

// Timing thresholds
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

#endif
```
</details>

<details>
<summary><b>📁 inc/adc.h</b></summary>

```c
#ifndef ADC_H
#define ADC_H

void init_adc(void);
unsigned short read_adc(void);

#endif
```
</details>

<details>
<summary><b>📁 inc/clcd.h</b></summary>

```c
#ifndef CLCD_H
#define CLCD_H

#define _XTAL_FREQ                  20000000

#define CLCD_DATA_PORT_DDR          TRISD
#define CLCD_RS_DDR                 TRISE2
#define CLCD_EN_DDR                 TRISE1

#define CLCD_DATA_PORT              PORTD
#define CLCD_RS                     RE2
#define CLCD_EN                     RE1

#define INST_MODE                   0
#define DATA_MODE                   1

#define HI                          1
#define LOW                         0

#define LINE1(x)                    (0x80 + x)
#define LINE2(x)                    (0xC0 + x)

#define EIGHT_BIT_MODE              0x33
#define TWO_LINES_5x8_8_BIT_MODE    0x38
#define CLEAR_DISP_SCREEN           0x01
#define DISP_ON_AND_CURSOR_OFF      0x0C

void init_clcd(void);
void clcd_putch(const char data, unsigned char addr);
void clcd_print(const char *str, unsigned char addr);
void clcd_clear(void);

#endif
```
</details>

<details>
<summary><b>📁 inc/dkp.h</b></summary>

```c
#ifndef DKP_H
#define DKP_H

#define LEVEL_DETECTION         0
#define LEVEL                   0
#define STATE_DETECTION         1
#define STATE                   1

#define KEYPAD_PORT             PORTB
#define KEYPAD_PORT_DDR         TRISB

#define INPUT_LINES             0x3F

#define SW1                     0x3E
#define SW2                     0x3D
#define SW3                     0x3B
#define SW4                     0x37
#define SW5                     0x2F
#define SW6                     0x1F

#define ALL_RELEASED            INPUT_LINES

unsigned char read_digital_keypad(unsigned char mode);
void init_digital_keypad(void);

#endif
```
</details>

<details>
<summary><b>📁 inc/ds1307.h</b></summary>

```c
#ifndef DS1307_H
#define DS1307_H

#define SLAVE_WRITE             0b11010000
#define SLAVE_READ              0b11010001

#define SEC_ADDR                0x00
#define MIN_ADDR                0x01
#define HOUR_ADDR               0x02

void init_ds1307(void);
unsigned char read_ds1307(unsigned char addr);
void write_ds1307(unsigned char addr, unsigned char data);
void get_time(unsigned char *clock_reg);
void display_time(unsigned char *clock_reg, unsigned int index);

#endif
```
</details>

<details>
<summary><b>📁 inc/ext_eeprom.h</b></summary>

```c
#ifndef EEPROM_H
#define EEPROM_H

#define EEPROM_SLAVE_WRITE      0b10100000
#define EEPROM_SLAVE_READ       0b10100001

unsigned char read_ext_eeprom(unsigned char addr);
void write_ext_eeprom(unsigned char addr, unsigned char data);

#endif
```
</details>

<details>
<summary><b>📁 inc/i2c.h</b></summary>

```c
#ifndef I2C_H
#define I2C_H

#define FOSC            20000000

void init_i2c(unsigned long baud);
void i2c_start(void);
void i2c_rep_start(void);
void i2c_stop(void);
unsigned char i2c_read(unsigned char ack);
int i2c_write(unsigned char data);

#endif
```
</details>

<details>
<summary><b>📁 inc/password.h</b></summary>

```c
#ifndef PASSWORD_H
#define PASSWORD_H

int compare_pass(const char *p1, const char *p2);
int change_password(void);
unsigned int old_password(void);
unsigned int new_password(void);

#endif
```
</details>

<details>
<summary><b>📁 inc/uart.h</b></summary>

```c
#ifndef UART_H
#define UART_H

#define FOSC                20000000

void init_uart(unsigned long baud);
unsigned char getchar(void);
void putchar(unsigned char data);
void puts(const char *s);

#endif
```
</details>

<details>
<summary><b>📁 src/main.c</b></summary>

```c
#include <xc.h>
#include "main.h"

#pragma config WDTE = OFF
#define _XTAL_FREQ 20000000

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

        if (operation != MODE_MENU && key == SW4) 
        {
            operation = MODE_LOGIN;
        }

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

void init_password() 
{
    unsigned char i = 0x01;

    if(eeprom_read(0x00) != 0xAA) 
    {
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
        for(int k = 0; k < PASS_LEN; k++) 
        {
            correct_pass[k] = eeprom_read(i++);
        }
    }
}
```
</details>

<details>
<summary><b>📁 src/login.c</b></summary>

```c
#include <xc.h>
#include "main.h"
#include "config.h"

char correct_pass[PASS_LEN];
char user_pass[PASS_LEN];

unsigned int attempts = MAX_ATTEMPTS;
unsigned int index = 0;
unsigned int blink_freq = 0;
unsigned char blink_toggle = 0;
unsigned char is_blocked = 0;
unsigned char key;
unsigned int timeout = 0;
unsigned char disp_once = 1;

int compare_pass(const char *p1, const char *p2) 
{
    for (int i = 0; i < PASS_LEN; i++)
    {
        if (p1[i] != p2[i])
            return FAILURE;
    }
    return SUCCESS;
}

unsigned char login_screen(unsigned char login_mode) 
{
    if (is_blocked) 
    {
        clcd_print(" LOCKED 2 MIN   ", LINE1(0));
        clcd_print(" TRY LATER      ", LINE2(0));

        for (int i = 0; i < 20; i++) 
        {
            __delay_ms(6000);
        }

        is_blocked = 0;
        attempts = MAX_ATTEMPTS;
        index = 0;
        timeout = 0;
        disp_once = 1;
        return login_mode;
    }

    if (disp_once) 
    {
        clcd_clear();
        clcd_print("ENTER PASSWORD  ", LINE1(0));
        clcd_print("                ", LINE2(0));
        disp_once = 0;
    }

    if (index < PASS_LEN) 
    {
        if (blink_freq++ >= BLINK_FREQ_DIVIDER) 
        {
            blink_freq = 0;
            clcd_putch(blink_toggle ? '_' : 255, LINE2(index));
            blink_toggle ^= 1;
        }
    }

    key = read_digital_keypad(STATE);

    if (key != ALL_RELEASED) 
    {
        timeout = 0;

        if (index < PASS_LEN) 
        {
            if (key == SW4)
                user_pass[index] = 'x';
            else if (key == SW5)
                user_pass[index] = 'y';
            else
                return login_mode;

            clcd_putch('*', LINE2(index));
            index++;
            blink_toggle = 0;
            blink_freq = 0;
            __delay_ms(200); 
        }
    }

    if (index == PASS_LEN) 
    {
        blink_toggle = 0;

        if (compare_pass(user_pass, correct_pass) == SUCCESS) 
        {
            clcd_print("CORRECT PASSWORD", LINE1(0));
            clcd_print("LOGIN SUCCESSFUL", LINE2(0));
            __delay_ms(2000);
            clcd_clear();

            index = 0;
            disp_once = 1;
            timeout = 0;
            blink_freq = 0;
            blink_toggle = 0;
            attempts = MAX_ATTEMPTS;
            for (int i = 0; i < PASS_LEN; i++) 
                user_pass[i] = 0;

            return MODE_MENU;
        } 
        else 
        {
            attempts--;
            index = 0;
            disp_once = 1;
            blink_freq = 0;
            blink_toggle = 0;

            clcd_print(" WRONG PASSWORD ", LINE1(0));
            clcd_print("Attempts Left :  ", LINE2(0));
            clcd_putch(attempts + '0', LINE2(15));
            
            for (int i = 0; i < PASS_LEN; i++) 
                user_pass[i] = 0;

            __delay_ms(2000);

            if (attempts == 0)
                is_blocked = 1;
        }
    }

    timeout++;
    if (timeout > TIMEOUT_5SEC) 
    {
        login_mode = MODE_DASHBOARD;
        timeout = 0;
        index = 0;
        blink_freq = 0;
        blink_toggle = 0;
        attempts = MAX_ATTEMPTS;
        disp_once = 1;

        for (int i = 0; i < PASS_LEN; i++) 
            user_pass[i] = 0;

        return MODE_DASHBOARD;
    }

    __delay_us(100);
    return login_mode;
}
```
</details>

<details>
<summary><b>📁 src/dashboard.c</b></summary>

```c
#include <xc.h>
#include "main.h"

unsigned char clock_reg[3];
unsigned char pos = 0;
unsigned short adc_value;
unsigned char speed[3] = {0};
const char *event[] = {"ON", "GN", "G1", "G2", "G3", "G4", "G5", "GR", "C "};

void dashboard(unsigned char key) 
{
    clcd_print("  TIME    EV  SP", LINE1(0));

    get_time(clock_reg);
    display_time(clock_reg, 0);

    if (key == SW1) 
    {
        pos = 8;
    }
    else if (key == SW2) 
    {
        if (pos < 7) 
        {
            ++pos;
        }
    }
    else if (key == SW3) 
    {
        if (pos > 1) 
        {
            --pos;
        }
    }

    clcd_print(event[pos], LINE2(10));

    adc_value = read_adc();
    adc_value = (adc_value) / 10.23;

    speed[1] = (adc_value % 10) + '0';
    speed[0] = (adc_value / 10) + '0';

    clcd_print(speed, LINE2(14));

    if (key == SW1 || key == SW2 || key == SW3) 
    {
        store_event(clock_reg, event[pos], speed);
    }
}
```
</details>

<details>
<summary><b>📁 src/menu.c</b></summary>

```c
#include <xc.h>
#include "main.h"
#include "config.h"

unsigned int menu_selector = 0;
unsigned char cursor = 1;
unsigned char key;

char *menu_e[] = {" View log      ", " Clear log     ", " Download log  ", 
                  " Set Time      ", " Change Passwd "};

unsigned int sw4_time = 0, sw5_time = 0;
unsigned char reset1 = 0, reset2 = 0;

unsigned char menu(unsigned char operation) 
{
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

    clcd_print(menu_e[menu_selector], LINE1(1));
    clcd_print(menu_e[menu_selector + 1], LINE2(1));

    key = read_digital_keypad(LEVEL);

    if (key == SW4) 
    {
        sw4_time++;

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

    if (key == SW5)
    {
        sw5_time++;

        if (sw5_time > LONG_PRESS_THRESHOLD && reset2 == 0)
        {
            reset2 = 1;

            unsigned char selected_index = menu_selector + (cursor - 1);

            switch (selected_index)
            {
                case 0:
                    view_log_screen();
                    return MODE_DASHBOARD;
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
            }
        }
    }
    else
    {
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
```
</details>

<details>
<summary><b>📁 src/log.c</b></summary>

```c
#include "main.h"
#include "config.h"

static unsigned char log_index = 0;
static unsigned char log_count = 0;

unsigned char get_log_index(void)
{
    return log_index;
}

unsigned char get_log_count(void)
{
    return log_count;
}

void clear_all_logs(void)
{
    log_index = 0;
    log_count = 0;
}

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

void store_event(unsigned char *clock_reg, char *event, char *speed)
{
    static unsigned char addr;
    unsigned char i;
    char time[8];

    time[0] = ((clock_reg[0] >> 4) & 0x03) + '0';
    time[1] = (clock_reg[0] & 0x0F) + '0';
    time[2] = ((clock_reg[1] >> 4) & 0x07) + '0';
    time[3] = (clock_reg[1] & 0x0F) + '0';
    time[4] = ((clock_reg[2] >> 4) & 0x07) + '0';
    time[5] = (clock_reg[2] & 0x0F) + '0';
    time[6] = ' ';
    time[7] = ' ';

    addr = EEPROM_LOG_START + (log_index * LOG_SIZE_BYTES);

    for (i = 0; i < 8; i++)
    {
        write_ext_eeprom(addr++, time[i]);
        __delay_ms(5);
    }

    write_ext_eeprom(addr++, event[0]); 
    __delay_ms(5);
    write_ext_eeprom(addr++, event[1]); 
    __delay_ms(5);

    write_ext_eeprom(addr++, speed[0]); 
    __delay_ms(5);
    write_ext_eeprom(addr++, speed[1]); 
    __delay_ms(5);

    log_index++;
    
    if (log_index == MAX_LOG_ENTRIES)
        log_index = 0;

    if (log_count < MAX_LOG_ENTRIES)
        log_count++;
}
```
</details>

<details>
<summary><b>📁 src/view_log.c</b></summary>

```c
#include <xc.h>
#include "uart.h"
#include "main.h"
#include "config.h"

#pragma config WDTE = OFF
#define _XTAL_FREQ 20000000

void init_conf() 
{
    init_uart(UART_BAUD_RATE);
}

void download_log(void) 
{
    init_conf();
    unsigned char addr = EEPROM_LOG_START;
    unsigned char ch;
    unsigned char log_count = get_log_count();

    clcd_print("Tx Logs UART  ", LINE1(0));
    clcd_print("Please Wait...", LINE2(0));

    putchar('\r');
    putchar('\n');
    puts("ID  TIME    EV SP");
    putchar('\r');
    putchar('\n');

    for (unsigned char log_no = 0; log_no < log_count; log_no++) 
    {
        putchar('0' + log_no);
        putchar(' ');

        for (unsigned char i = 0; i < LOG_SIZE_BYTES; i++) 
        {
            ch = read_ext_eeprom(addr++);
            putchar(ch);
            
            if ((i + 1) == 2 || (i + 1) == 4) 
            {
                putchar(':');
            } 
            else if ((i + 1) == 10) 
            {
                putchar(' ');
            }
            __delay_ms(2);
        }

        putchar('\r');
        putchar('\n');
    }

    clcd_print("Done", LINE2(5));
    clcd_clear();
    __delay_ms(2000);
}

void view_log_screen(void) 
{
    unsigned char current = 0;
    unsigned char key_level;
    unsigned char key_state;
    char log[LOG_SIZE_BYTES + 1];
    char display[15];
    unsigned char index;
    unsigned char log_index_local, log_count_local;
    
    unsigned int sw4_press_time = 0;
    unsigned char sw4_pressed = 0;

    log_index_local = get_log_index();
    log_count_local = get_log_count();

    if (log_count_local == 0) 
    {
        clcd_print("NO LOG AVAILABLE", LINE1(0));
        clcd_print("                ", LINE2(0));
        __delay_ms(2000);
        return;
    }

    while (1) 
    {
        index = (log_index_local + MAX_LOG_ENTRIES - 1 - current) % MAX_LOG_ENTRIES;

        read_event(index, log);
        log[LOG_SIZE_BYTES] = '\0';

        display[0] = log[0];
        display[1] = log[1];
        display[2] = ':';
        display[3] = log[2];
        display[4] = log[3];
        display[5] = ':';
        display[6] = log[4];
        display[7] = log[5];
        display[8] = ' ';
        display[9] = log[8];
        display[10] = log[9];
        display[11] = ' ';
        display[12] = log[10];
        display[13] = log[11];
        display[14] = '\0';

        clcd_print("  TIME   EV SP", LINE1(2));
        clcd_print(display, LINE2(2));

        key_level = read_digital_keypad(LEVEL);
        key_state = read_digital_keypad(STATE);

        if (key_level == SW4)
        {
            if (!sw4_pressed)
            {
                sw4_pressed = 1;
                sw4_press_time = 0;
            }
            else
            {
                sw4_press_time++;
                
                if (sw4_press_time >= LONG_PRESS_THRESHOLD)
                {
                    clcd_clear();
                    return;
                }
            }
        }
        else
        {
            if (sw4_pressed)
            {
                if (sw4_press_time < LONG_PRESS_THRESHOLD && current < log_count_local - 1)
                {
                    current++;
                }
                sw4_pressed = 0;
                sw4_press_time = 0;
            }
        }

        if (key_state == SW5 && current > 0)
        {
            current--;
            __delay_ms(200);
        }

        __delay_ms(10);
    }
}
```
</details>

<details>
<summary><b>📁 src/set_time.c</b></summary>

```c
#include <xc.h>
#include "main.h"
#include "config.h"

unsigned int blink_freq = 0;
unsigned char blink_toggle = 0;

unsigned char dec_to_bcd(unsigned char dec)
{
    return ((dec/10) << 4) | (dec % 10);
}

void set_time_screen(void)
{
    unsigned char key;
    unsigned char field = 0;
    unsigned int sw4_time = 0;
    unsigned char reset4 = 0;
    unsigned int sw5_time = 0;
    unsigned char reset5 = 0;
    unsigned char clock_reg[3];

    clcd_clear();
    get_time(clock_reg);  

    while (1)
    {
        key = read_digital_keypad(LEVEL);

        clcd_print("SET     ", LINE1(0));
        clcd_print("TIME    ", LINE2(0));

        if (blink_freq++ >= 3) 
        {
            blink_freq = 0;
            
            if (field == 0)
            {
                clcd_putch(blink_toggle ? ' ' : (clock_reg[0] >> 4) + '0', LINE2(8));
                clcd_putch(blink_toggle ? ' ' : (clock_reg[0] & 0x0F) + '0', LINE2(9));
            }
            else if (field == 1)
            {
                clcd_putch(blink_toggle ? ' ' : (clock_reg[1] >> 4) + '0', LINE2(11));
                clcd_putch(blink_toggle ? ' ' : (clock_reg[1] & 0x0F) + '0', LINE2(12));
            }
            else if (field == 2)
            {
                clcd_putch(blink_toggle ? ' ' : (clock_reg[2] >> 4) + '0', LINE2(14));
                clcd_putch(blink_toggle ? ' ' : (clock_reg[2] & 0x0F) + '0', LINE2(15));
            }
            blink_toggle ^= 1;
        }
        else
        {
            display_time(clock_reg, 8);
        }

        if (key == SW4)
        {
            sw4_time++;

            if (sw4_time > LONG_PRESS_THRESHOLD && reset4 == 0)
            {
                reset4 = 1;
                clcd_clear();
                return;
            }
        }
        else
        {
            if (sw4_time > 0 && sw4_time <= SHORT_PRESS_THRESHOLD && reset4 == 0)
            {
                unsigned char value;

                if (field == 0)
                {
                    value = ((clock_reg[0] >> 4) * 10) + (clock_reg[0] & 0x0F);
                    value = (value == 0) ? 23 : value - 1;
                    clock_reg[0] = dec_to_bcd(value);
                }
                else if (field == 1)
                {
                    value = ((clock_reg[1] >> 4) * 10) + (clock_reg[1] & 0x0F);
                    value = (value == 0) ? 59 : value - 1;
                    clock_reg[1] = dec_to_bcd(value);
                }
                else if (field == 2)
                {
                    value = ((clock_reg[2] >> 4) * 10) + (clock_reg[2] & 0x0F);
                    value = (value == 0) ? 59 : value - 1;
                    clock_reg[2] = dec_to_bcd(value);
                }
            }

            sw4_time = 0;
            reset4 = 0;
        }

        if (key == SW5)
        {
            sw5_time++;

            if (sw5_time > LONG_PRESS_THRESHOLD && reset5 == 0)
            {
                reset5 = 1;
                field++;

                if (field > 2)
                {
                    write_ds1307(HOUR_ADDR, clock_reg[0]);
                    write_ds1307(MIN_ADDR,  clock_reg[1]);
                    write_ds1307(SEC_ADDR,  clock_reg[2] & 0x7F);

                    clcd_clear();
                    clcd_print(" TIME UPDATED   ", LINE1(0));
                    __delay_ms(1500);
                    return;
                }
            }
        }
        else
        {
            if (sw5_time > 0 && sw5_time <= SHORT_PRESS_THRESHOLD && reset5 == 0)
            {
                unsigned char value;

                if (field == 0)
                {
                    value = ((clock_reg[0] >> 4) * 10) + (clock_reg[0] & 0x0F);
                    value = (value + 1) % 24;
                    clock_reg[0] = dec_to_bcd(value);
                }
                else if (field == 1)
                {
                    value = ((clock_reg[1] >> 4) * 10) + (clock_reg[1] & 0x0F);
                    value = (value + 1) % 60;
                    clock_reg[1] = dec_to_bcd(value);
                }
                else if (field == 2)
                {
                    value = ((clock_reg[2] >> 4) * 10) + (clock_reg[2] & 0x0F);
                    value = (value + 1) % 60;
                    clock_reg[2] = dec_to_bcd(value);
                }
            }

            sw5_time = 0;
            reset5 = 0;
        }
    }
}
```
</details>

<details>
<summary><b>📁 src/change_password.c</b></summary>

```c
#include <xc.h>
#include "main.h"
#include "config.h"

char user_pass[PASS_LEN];

unsigned int attempts = MAX_ATTEMPTS;
unsigned int index = 0;
unsigned int blink_freq = 0;
unsigned char blink_toggle = 0;
unsigned char is_blocked = 0;
unsigned char key;
unsigned int timeout = 0;
unsigned char disp_once = 1;

int change_password() 
{
    unsigned int ret_old_pass = 0, ret_new_pass = 0;
    
    while ((ret_old_pass = old_password()) == CONTINUE) 
    {
    }

    if (ret_old_pass == SUCCESS) 
    {
        while((ret_new_pass = new_password()) == CONTINUE)
        {
        }
        
        if(ret_new_pass == SUCCESS)
        {
            return SUCCESS;
        }
    }
    
    return SUCCESS;
}

unsigned int old_password() 
{
    if (disp_once) 
    {
        clcd_print(" ENTER OLD PASS ", LINE1(0));
        clcd_print("                ", LINE2(0));
        disp_once = 0;
        __delay_ms(1000);
    }

    if (index < PASS_LEN) 
    {
        if (blink_freq++ >= BLINK_FREQ_DIVIDER) 
        {
            blink_freq = 0;
            clcd_putch(blink_toggle ? '_' : 255, LINE2(index));
            blink_toggle ^= 1;
        }
    }

    key = read_digital_keypad(STATE);

    if (key != ALL_RELEASED) 
    {
        timeout = 0;

        if (index < PASS_LEN) 
        {
            if (key == SW4)
                user_pass[index] = 'x';
            else if (key == SW5)
                user_pass[index] = 'y';
            else
                return CONTINUE;

            clcd_putch('*', LINE2(index));
            index++;

            blink_toggle = 0;
            blink_freq = 0;
            __delay_ms(200);
        }
    }

    if (index == PASS_LEN) 
    {
        blink_toggle = 0;

        if (compare_pass(user_pass, correct_pass) == SUCCESS) 
        {
            clcd_print("CORRECT PASSWORD", LINE1(0));
            clcd_print("LOGIN SUCCESSFUL", LINE2(0));
            __delay_ms(2000);
            clcd_clear();

            index = 0;
            disp_once = 1;
            timeout = 0;
            blink_freq = 0;
            blink_toggle = 0;
            attempts = MAX_ATTEMPTS;
            for (int i = 0; i < PASS_LEN; i++) 
                user_pass[i] = 0;

            return SUCCESS;
        }
        else 
        {
            attempts--;
            index = 0;
            disp_once = 1;
            blink_freq = 0;
            blink_toggle = 0;

            clcd_print(" WRONG PASSWORD ", LINE1(0));
            clcd_print("Attempts Left :  ", LINE2(0));
            clcd_putch(attempts + '0', LINE2(15));

            for (int i = 0; i < PASS_LEN; i++) 
                user_pass[i] = 0;

            __delay_ms(2000);

            if (attempts == 0)
                is_blocked = 1;
        }
    }

    timeout++;
    if (timeout > TIMEOUT_5SEC) 
    {
        timeout = 0;
        index = 0;
        blink_freq = 0;
        blink_toggle = 0;
        attempts = MAX_ATTEMPTS;
        disp_once = 1;

        for (int i = 0; i < PASS_LEN; i++) 
            user_pass[i] = 0;
            
        return TIMEOUT;
    }

    __delay_us(100);
    return CONTINUE;
}

unsigned int new_password() 
{
    if (disp_once) 
    {
        clcd_print(" ENTER NEW PASS ", LINE1(0));
        clcd_print("                ", LINE2(0));
        disp_once = 0;
        __delay_us(1000);
    }

    if (index < PASS_LEN) 
    {
        if (blink_freq++ >= BLINK_FREQ_DIVIDER) 
        {
            blink_freq = 0;
            clcd_putch(blink_toggle ? '_' : 255, LINE2(index));
            blink_toggle ^= 1;
        }
    }

    key = read_digital_keypad(STATE);

    if (key != ALL_RELEASED) 
    {
        timeout = 0;

        if (index < PASS_LEN) 
        {
            if (key == SW4)
                user_pass[index] = 'x';
            else if (key == SW5)
                user_pass[index] = 'y';
            else
                return CONTINUE;

            clcd_putch('*', LINE2(index));
            index++;

            blink_toggle = 0;
            blink_freq = 0;
            __delay_ms(200);
        }
    }

    if (index == PASS_LEN) 
    {
        blink_toggle = 0;

        clcd_print(" PASSWORD CHANGED", LINE1(0));
        clcd_print("   SUCCESSFULLY  ", LINE2(0));
        __delay_ms(2000);
        clcd_clear();

        eeprom_write(0x00, 0xAA);
        unsigned char i = 0x01;
        for (int k = 0; k < PASS_LEN; k++) 
        {
            eeprom_write(i++, user_pass[k]);
        }
        
        init_password();

        index = 0;
        disp_once = 1;
        timeout = 0;
        blink_freq = 0;
        blink_toggle = 0;
        for (int i = 0; i < PASS_LEN; i++) 
            user_pass[i] = 0;

        return SUCCESS;
    }

    timeout++;
    if (timeout > TIMEOUT_5SEC) 
    {
        timeout = 0;
        index = 0;
        blink_freq = 0;
        blink_toggle = 0;
        disp_once = 1;

        for (int i = 0; i < PASS_LEN; i++) 
            user_pass[i] = 0;
            
        return TIMEOUT;
    }

    __delay_us(100);
    return CONTINUE;
}
```
</details>

<details>
<summary><b>📁 drivers/adc.c</b></summary>

```c
#include <xc.h>
#include "adc.h"

void init_adc(void)
{
    ADFM = 1;
    ADON = 1; 
    ADCS2 = 0;
    ADCS1 = 1;
    ADCS0 = 0;
    CHS2 = 0;
    CHS1 = 0;
    CHS0 = 0;
    PCFG3 = 0;
    PCFG2 = 0;
    PCFG1 = 0;
    PCFG0 = 0;
    ADRESH = 0x00;
    ADRESL = 0x00;
}

unsigned short read_adc(void)
{
    unsigned short adc_reg_val;
    
    GO = 1;
    while (nDONE);
   
    adc_reg_val = (ADRESH << 8) | ADRESL;
    
    return adc_reg_val;
}
```
</details>

<details>
<summary><b>📁 drivers/clcd.c</b></summary>

```c
#include <xc.h>
#include "clcd.h"

static void clcd_write(unsigned char byte, unsigned char mode)
{
    CLCD_RS = mode;
    CLCD_DATA_PORT = byte;
    
    CLCD_EN = HI;
    __delay_us(100);
    CLCD_EN = LOW;
    
    __delay_us(4100);
}

static void init_display_controller(void)
{
    __delay_ms(30);
    
    clcd_write(EIGHT_BIT_MODE, INST_MODE);
    __delay_us(4100);
    clcd_write(EIGHT_BIT_MODE, INST_MODE);
    __delay_us(100);
    clcd_write(EIGHT_BIT_MODE, INST_MODE);
    __delay_us(1); 
    
    clcd_write(TWO_LINES_5x8_8_BIT_MODE, INST_MODE);
    __delay_us(100);
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
    __delay_us(500);
    clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
    __delay_us(100);
}

void init_clcd(void)
{
    CLCD_DATA_PORT_DDR = 0x00;
    CLCD_RS_DDR = 0;
    CLCD_EN_DDR = 0;
    
    init_display_controller();
}

void clcd_putch(const char data, unsigned char addr)
{
    clcd_write(addr, INST_MODE);
    clcd_write(data, DATA_MODE);
}

void clcd_print(const char *str, unsigned char addr)
{
    clcd_write(addr, INST_MODE);
    
    while (*str != '\0')
    {
        clcd_write(*str, DATA_MODE); 
        str++;
    }
}

void clcd_clear() {
    clcd_print("                ", LINE1(0));
    clcd_print("                ", LINE2(0));
}
```
</details>

<details>
<summary><b>📁 drivers/dkp.c</b></summary>

```c
#include <xc.h>
#include "dkp.h"

void init_digital_keypad(void)
{
    KEYPAD_PORT_DDR = KEYPAD_PORT_DDR | INPUT_LINES;
}

unsigned char read_digital_keypad(unsigned char mode)
{
    static unsigned char once = 1;
    
    if (mode == LEVEL_DETECTION)
    {
        return KEYPAD_PORT & INPUT_LINES;
    }
    else
    {
        if (((KEYPAD_PORT & INPUT_LINES) != ALL_RELEASED) && once)
        {
            once = 0;
            return KEYPAD_PORT & INPUT_LINES;
        }
        else if ((KEYPAD_PORT & INPUT_LINES) == ALL_RELEASED)
        {
            once = 1;
        }
    }
    
    return ALL_RELEASED;
}
```
</details>

<details>
<summary><b>📁 drivers/ds1307.c</b></summary>

```c
#include <xc.h>
#include "i2c.h"
#include "main.h"

void init_ds1307(void)
{
    unsigned char dummy;
   
    dummy = read_ds1307(SEC_ADDR);
    dummy = dummy & 0x7F;
    write_ds1307(SEC_ADDR, dummy);
}

unsigned char read_ds1307(unsigned char addr)
{
    unsigned char data;
    
    i2c_start();
    i2c_write(SLAVE_WRITE);
    i2c_write(addr);
    i2c_rep_start();
    i2c_write(SLAVE_READ);
    data = i2c_read(0);
    i2c_stop();
    
    return data;
}

void write_ds1307(unsigned char addr, unsigned char data)
{
    i2c_start();
    i2c_write(SLAVE_WRITE);
    i2c_write(addr);
    i2c_write(data);
    i2c_stop();
}

void get_time(unsigned char *clock_reg)
{
    clock_reg[0] = read_ds1307(HOUR_ADDR);
    clock_reg[1] = read_ds1307(MIN_ADDR);
    clock_reg[2] = read_ds1307(SEC_ADDR);
}

void display_time(unsigned char *clock_reg, unsigned int index)
{
    char time[9];
    
    time[0] = ((clock_reg[0] >> 4) & 0x03) + '0';
    time[1] = (clock_reg[0] & 0x0F) + '0';
    time[2] = ':';
    time[3] = ((clock_reg[1] >> 4) & 0x07) + '0';
    time[4] = (clock_reg[1] & 0x0F) + '0';
    time[5] = ':';
    time[6] = ((clock_reg[2] >> 4) & 0x07) + '0';
    time[7] = (clock_reg[2] & 0x0F) + '0';
    time[8] = '\0';
    
    clcd_print(time, LINE2(index));
}
```
</details>

<details>
<summary><b>📁 drivers/ext_eeprom.c</b></summary>

```c
#include <xc.h>
#include "i2c.h"
#include "ext_eeprom.h"

unsigned char read_ext_eeprom(unsigned char addr)
{
    unsigned char data;
    
    i2c_start();
    i2c_write(EEPROM_SLAVE_WRITE);
    i2c_write(addr);
    i2c_rep_start();
    i2c_write(EEPROM_SLAVE_READ);
    data = i2c_read(0);
    i2c_stop();
    
    return data;
}

void write_ext_eeprom(unsigned char addr, unsigned char data)
{
    i2c_start();
    i2c_write(EEPROM_SLAVE_WRITE);
    i2c_write(addr);
    i2c_write(data);
    i2c_stop();
    for(int delay = 0; delay < 100; delay++);
}
```
</details>

<details>
<summary><b>📁 drivers/i2c.c</b></summary>

```c
#include <xc.h>
#include "i2c.h"

void init_i2c(unsigned long baud)
{
    SSPM3 = 1;
    SSPADD = (unsigned char)(FOSC / (4 * baud)) - 1;
    SSPEN = 1;
}

static void i2c_wait_for_idle(void)
{
    while (R_nW || (SSPCON2 & 0x1F));
}

void i2c_start(void)
{
    i2c_wait_for_idle();
    SEN = 1;
}

void i2c_rep_start(void)
{
    i2c_stop();
    i2c_start();
}

void i2c_stop(void)
{
    i2c_wait_for_idle();
    PEN = 1;
}

unsigned char i2c_read(unsigned char ack)
{
    unsigned char data;
    
    i2c_wait_for_idle();
    RCEN = 1;
    i2c_wait_for_idle();
    data = SSPBUF;
    
    if (ack == 1)
        ACKDT = 1;
    else
        ACKDT = 0;
  
    ACKEN = 1;
    
    return data;
}

int i2c_write(unsigned char data)
{
    i2c_wait_for_idle();
    SSPBUF = data;
    
    return !ACKSTAT;
}
```
</details>

<details>
<summary><b>📁 drivers/uart.c</b></summary>

```c
#include <xc.h>
#include "uart.h"

void init_uart(unsigned long baud)
{
    SPEN = 1;
    CREN = 1;
    SPBRG = (FOSC / (16 * baud)) - 1;
}

unsigned char getchar(void)
{
    while (RCIF != 1)
    {
        continue;
    }
    
    RCIF = 0;
    return RCREG;
}

void putchar(unsigned char data)
{
    TXREG = data;
    
    do
    {
        continue;
    } while (TXIF != 1);
    
    TXIF = 0;
}

void puts(const char *s)
{
    while (*s)
    {
        putchar(*s++);
    }
}
```
</details>

---

## 👨‍💻 Author

**Labhesh Patil**
- GitHub: [@LABHESHP2003](https://github.com/LABHESHP2003)
- Project Link: [https://github.com/LABHESHP2003/Car_Black_Box](https://github.com/LABHESHP2003/Car_Black_Box)

---

## 📄 License

MIT License © 2025 Labhesh Patil

---

<p align="center">
  <b>Made with ❤️ for embedded systems</b>
</p>