# Car Black Box - PIC16F877A (PIC Genuine Board)

[![PIC Microcontroller](https://img.shields.io/badge/MCU-PIC16F877A-blue.svg)](https://www.microchip.com/)
[![Language](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![I2C](https://img.shields.io/badge/Interface-I2C-orange.svg)](https://en.wikipedia.org/wiki/I%C2%B2C)
[![RTC](https://img.shields.io/badge/RTC-DS1307-green.svg)](https://www.maximintegrated.com/en/products/analog/real-time-clocks/DS1307.html)

## 📋 Table of Contents
- [Overview](#-overview)
- [Features](#-features)
- [Hardware Requirements](#-hardware-requirements)
- [PIC Genuine Board Pin Configuration](#-pic-genuine-board-pin-configuration)
- [Project Structure](#-project-structure)
- [Software Requirements](#-software-requirements)
- [Installation Guide](#-installation-guide)
- [User Manual](#-user-manual)
- [Menu System](#-menu-system)
- [Operation Guide](#-operation-guide)
- [Configuration](#-configuration)
- [Troubleshooting](#-troubleshooting)
- [Technical Specifications](#-technical-specifications)
- [Author](#-author)

## 📋 Overview

A comprehensive Car Black Box system implemented on **PIC16F877A** microcontroller (PIC Genuine Board). This project records vehicle events (gear changes, collisions) with timestamps and stores them in external EEPROM, similar to an aircraft black box. The system features password-protected access, real-time clock, and log viewing/download capabilities.

The system continuously monitors gear position (simulated via keypad), speed (via potentiometer), and time (via DS1307 RTC). Any change in gear or collision event is automatically logged with timestamp and speed to external EEPROM. Authorized personnel can access these logs through a password-protected menu system.

## 🎯 Features

### Real-time Dashboard
- **Current Time Display**: Shows HH:MM:SS format from DS1307 RTC
- **Gear Position Indicator**: Displays ON, GN, G1-G5, GR (Reverse), C (Collision)
- **Speed Display**: Real-time speed from 0-100 km/h via potentiometer (ADC)
- **Collision Event**: Special event marker triggered by SW1

### Secure Access System
- **4-digit Password Protection**: Uses 'x' (SW4) and 'y' (SW5) characters
- **3 Attempt Limit**: System locks for 2 minutes after 3 failed attempts
- **Password Change**: Authorized users can change password
- **Non-volatile Storage**: Password stored in internal EEPROM
- **Auto Timeout**: Returns to dashboard after 5 seconds of inactivity

### Event Logging System
- **Automatic Logging**: Events recorded on any key press (gear change/collision)
- **Circular Buffer**: Stores last 10 events in external EEPROM
- **Complete Information**: Each log entry contains:
  - Time: HH:MM:SS (8 bytes)
  - Event: 2 characters (e.g., "G3", "C ")
  - Speed: 2 digits (0-99 km/h)
- **Non-volatile**: Data retained even after power loss

### Log Management
- **View Logs**: Scroll through recorded events on LCD
- **Clear Logs**: Erase all stored events
- **Download Logs**: Transmit all logs via UART (9600 baud)
- **Formatted Output**: CSV-style format for easy parsing

### Time Management
- **Set Time**: User can adjust RTC time
- **Field Selection**: Select hours, minutes, or seconds
- **Blinking Cursor**: Visual feedback for current field
- **Increment/Decrement**: SW5 increase, SW4 decrease
- **Save/Cancel**: Long press SW5 to save, long press SW4 to cancel

## 🛠️ Hardware Requirements

### PIC Genuine Board Components
| Component | Specification | Quantity |
|-----------|--------------|----------|
| **Microcontroller** | PIC16F877A | 1 (onboard) |
| **LCD Display** | 16x2 Character LCD | 1 |
| **RTC Module** | DS1307 + CR2032 Battery | 1 |
| **EEPROM** | 24C04 or 24C08 | 1 |
| **Keypad** | Tactile Switches | 6 |
| **Potentiometer** | 10kΩ Linear | 1 (for speed) |
| **Potentiometer** | 10kΩ | 1 (LCD contrast) |
| **Crystal** | 20 MHz | 1 (onboard) |
| **Crystal** | 32.768 kHz | 1 (for RTC) |
| **Resistors** | 10kΩ | 8 (pull-ups) |
| **Capacitors** | 22pF | 4 (for crystals) |
| **Battery** | CR2032 | 1 (for RTC backup) |

## 🔌 PIC Genuine Board Pin Configuration

### Default PIC Genuine Board Connections

| Port Pin | Function | Connected To | Description |
|----------|----------|--------------|-------------|
| **RA0** | AN0 | 10kΩ Potentiometer | Speed sensor input (0-5V) |
| **RB0** | Digital Input | SW1 | Collision/Event trigger |
| **RB1** | Digital Input | SW2 | Gear up |
| **RB2** | Digital Input | SW3 | Gear down |
| **RB3** | Digital Input | SW4 | Menu up / 'x' character |
| **RB4** | Digital Input | SW5 | Menu down / 'y' character |
| **RB5** | Digital Input | SW6 | Reserved |
| **RC3** | SCL (I2C) | DS1307 SCL + 24C04 SCL | I2C Clock line |
| **RC4** | SDA (I2C) | DS1307 SDA + 24C04 SDA | I2C Data line |
| **RC6** | TX (UART) | MAX232/USB-TTL | Transmit to PC |
| **RC7** | RX (UART) | MAX232/USB-TTL | Receive from PC |
| **RD0-RD7** | Output | LCD D0-D7 | LCD data lines |
| **RE1** | Output | LCD EN | LCD enable signal |
| **RE2** | Output | LCD RS | LCD register select |
| **OSC1/OSC2** | - | 20 MHz Crystal | System clock (onboard) |
| **MCLR** | - | Programming header | Master clear |

### External Connections

#### I2C Devices (DS1307 + 24C04)
```
PIC16F877A          DS1307          24C04
RC3 (SCL) --------> SCL ----------> SCL
RC4 (SDA) --------> SDA ----------> SDA
GND --------------> GND ----------> VSS (pin 4)
+5V --------------> VCC ----------> VCC (pin 8)
                    X1, X2 - 32.768 kHz crystal
                    VBAT - CR2032 battery (+)
                     
24C04 Address: A0-A2 connected to GND (address 0x50)
```

#### LCD 16x2 Connections
```
LCD Pin    Function    PIC Pin
1 (VSS)    GND         GND
2 (VDD)    +5V         +5V
3 (V0)     Contrast    10kΩ pot to GND
4 (RS)     Register Select    RE2
5 (RW)     Read/Write  GND (tied low)
6 (EN)     Enable      RE1
7-14 (D0-D7) Data      RD0-RD7
15 (LED+)  Backlight   +5V (with 220Ω)
16 (LED-)  Backlight   GND
```

#### Keypad Connections
```
Switch    PIC Pin    Function
SW1       RB0        Collision
SW2       RB1        Gear Up
SW3       RB2        Gear Down
SW4       RB3        Menu Up / 'x' character
SW5       RB4        Menu Down / 'y' character
SW6       RB5        Reserved

All switches: Other side connected to GND
Pull-up: 10kΩ resistors to +5V on each RB pin
```

#### Speed Sensor (Potentiometer)
```
10kΩ Potentiometer:
Pin 1: +5V
Pin 2: RA0 (ADC input)
Pin 3: GND
```

#### UART Connection (for PC communication)
```
PIC16F877A    MAX232        DB9 Connector
RC6 (TX) ---> T2IN -------> T2OUT ---> Pin 2 (RX)
RC7 (RX) ---> R2OUT <------ R2IN  ---> Pin 3 (TX)
GND --------> GND --------> GND   ---> Pin 5

Or use USB-TTL converter:
RC6 (TX) ---> USB-TTL RX
RC7 (RX) ---> USB-TTL TX
GND --------> USB-TTL GND
```

## 📁 Project Structure

```
Car_Black_Box/
│
├── inc/                           # Header files
│   ├── main.h                     # Main header with operation modes
│   ├── config.h                   # System configuration constants
│   ├── adc.h                      # ADC driver header
│   ├── clcd.h                     # LCD driver header
│   ├── dkp.h                      # Digital keypad header
│   ├── ds1307.h                   # RTC driver header
│   ├── ext_eeprom.h               # External EEPROM header
│   ├── i2c.h                      # I2C driver header
│   ├── password.h                 # Password management header
│   └── uart.h                     # UART driver header
│
├── src/                           # Application source files
│   ├── main.c                     # Main program and initialization
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
│   ├── clcd.c                     # LCD driver implementation
│   ├── dkp.c                      # Digital keypad driver
│   ├── ds1307.c                   # RTC driver implementation
│   ├── ext_eeprom.c               # External EEPROM driver
│   ├── i2c.c                      # I2C master driver
│   └── uart.c                     # UART driver implementation
│
└── README.md                      # This file
```

## 💻 Software Requirements

### Development Tools
- **MPLAB X IDE**: v5.40 or later (Download: https://www.microchip.com/mplab/mplab-x-ide)
- **XC8 Compiler**: v2.32 or later (Download: https://www.microchip.com/mplab/compilers)
- **PICkit3/4**: Programmer/Debugger
- **Terminal Software**: PuTTY, TeraTerm, or Arduino Serial Monitor for UART

### Compiler Settings
```
Processor: PIC16F877A
Oscillator: HS (20 MHz)
Watchdog Timer: Disabled
Power-up Timer: Enabled
Brown-out Reset: Enabled
Low-Voltage Programming: Disabled
Data EE Read: Disabled
Code Protect: Off
```

## 📥 Installation Guide

### Step 1: Install Software
1. Download and install MPLAB X IDE
2. Download and install XC8 Compiler
3. Ensure compiler path is added in MPLAB

### Step 2: Download Project Files
Create all files with the content provided above in their respective directories.

### Step 3: Create Project in MPLAB X
1. Launch MPLAB X IDE
2. Click **File → New Project**
3. Select **Microchip Embedded → Standalone Project**
4. Click Next
5. Select Device: **PIC16F877A**
6. Select Tool: **PICkit3** (or your programmer)
7. Select Compiler: **XC8**
8. Enter Project Name: **Car_Black_Box**
9. Click Finish

### Step 4: Add Files to Project
1. Right-click on **Source Files** → Add Existing Item
2. Navigate to your src folder and select all .c files
3. Right-click on **Header Files** → Add Existing Item
4. Navigate to your inc folder and select all .h files
5. Right-click on **Important Files** → Add Existing Item
6. Navigate to drivers folder and select all driver .c files

### Step 5: Configure Project Properties
1. Right-click on project → **Properties**
2. Under **XC8 Compiler → Additional options** add: `-std=c99 -O1`

### Step 6: Build and Program
1. Click **Clean and Build** (Shift+F11)
2. Verify "BUILD SUCCESSFUL"
3. Connect PICkit3 to board
4. Click **Make and Program Device** (F6)

## 📖 User Manual

### First Power-On
1. Connect 5V power supply to PIC Genuine Board
2. LCD displays "ENTER PASSWORD"
3. Default password: **xyxy**
4. Use SW4 for 'x', SW5 for 'y'
5. Press 4 characters, system verifies
6. On success, enters dashboard mode

### Password Entry Tips
- Cursor (_) indicates current position
- Characters show as '*' for privacy
- Backspace not available - complete entry to verify
- 3 attempts before 2-minute lockout
- 5-second inactivity returns to dashboard

## 🎮 Operation Guide

### Dashboard Mode

**Display:**
```
  TIME    EV  SP
12:34:56  G3  45
```

**Controls:**
| Key | Action | Description |
|-----|--------|-------------|
| **SW1** | Collision | Records collision event (C ) |
| **SW2** | Gear Up | Increases gear (G1→G2→G3→G4→G5) |
| **SW3** | Gear Down | Decreases gear (G5→G4→G3→G2→G1→GN) |
| **SW4** | (Hold 2 sec) | Enter login screen |

**Gear States:**
- **ON**: Ignition ON
- **GN**: Neutral
- **G1-G5**: Gears 1-5
- **GR**: Reverse
- **C**: Collision event

### Menu System

To enter menu system:
1. From dashboard, press and hold SW4 (~2 seconds)
2. Enter password when prompted
3. On success, menu screen appears

**Menu Display:**
```
* View log      
  Clear log     
```

**Navigation:**
| Key | Action | Description |
|-----|--------|-------------|
| **SW4** | Short press | Move cursor up |
| **SW4** | Long press | Return to dashboard |
| **SW5** | Short press | Move cursor down |
| **SW5** | Long press | Select current option |

**Menu Options:**
1. **View log** - Browse recorded events
2. **Clear log** - Delete all stored logs
3. **Download log** - Transmit logs via UART
4. **Set Time** - Adjust RTC clock
5. **Change Passwd** - Update password

### View Log Mode

**Display:**
```
LOG 1/5  TIME   EV SP
12:34:56  G3  45
```

**Navigation:**
| Key | Action |
|-----|--------|
| **SW4** | Short press - Next log (newer) |
| **SW5** | Short press - Previous log (older) |
| **SW4** | Long press - Exit to dashboard |

### Set Time Mode

**Display:**
```
SET TIME
12:34:56
```

**Operation:**
| Key | Action |
|-----|--------|
| **SW4** | Short press - Decrease current field |
| **SW4** | Long press - Cancel, return to menu |
| **SW5** | Short press - Increase current field |
| **SW5** | Long press - Next field / Save |

Fields cycle: Hours → Minutes → Seconds → Save

### Change Password Mode

**Two-step process:**
1. Enter old password
2. Enter new password (automatically saved)

### Download Logs via UART

1. Connect PIC UART to PC (via USB-TTL)
2. Open terminal software (9600 baud, 8-N-1)
3. Select "Download log" from menu
4. Terminal receives:

```
ID  TIME    EV SP
0  12:34:56 G3 45
1  12:35:10 G4 60
2  12:35:45 G5 78
3  12:36:20 C  45
```

## ⚙️ Configuration

### config.h - System Parameters
```c
// Timing thresholds
#define LONG_PRESS_THRESHOLD    25   // ~1 second
#define SHORT_PRESS_THRESHOLD   5    // ~200ms
#define BLINK_FREQ_DIVIDER      2500 // Cursor blink rate
#define TIMEOUT_5SEC            50000 // 5-second timeout

// Log configuration
#define MAX_LOG_ENTRIES         10   // Circular buffer size
#define LOG_SIZE_BYTES          12   // Bytes per log entry
#define EEPROM_LOG_START        0x00 // Start address in EEPROM

// UART configuration
#define UART_BAUD_RATE          9600
```

### main.h - Operation Modes
```c
// Login configuration
#define MAX_ATTEMPTS        3
#define PASS_LEN            4

// Operation modes
#define MODE_DASHBOARD      0
#define MODE_LOGIN          1
#define MODE_MENU           2

// Return status
#define SUCCESS             1
#define FAILURE             0
#define TIMEOUT             2
#define CONTINUE            0
```

## 🔍 Troubleshooting

### Common Issues

| Problem | Solution |
|---------|----------|
| **LCD not displaying** | Adjust contrast pot, check connections RD0-RD7, RE1, RE2 |
| **LCD garbled** | Check +5V supply, add 10µF capacitor near PIC |
| **No key response** | Add 10kΩ pull-ups to RB0-RB5, check switch connections |
| **RTC not working** | Check 32.768 kHz crystal, add 22pF caps, check battery |
| **RTC losing time** | Replace CR2032 battery |
| **I2C not responding** | Add 10kΩ pull-ups to RC3/RC4, check addresses |
| **EEPROM write fails** | Connect WP pin to GND |
| **No UART output** | Check baud rate (9600), verify TX/RX connections |
| **Cannot enter menu** | Hold SW4 longer (>2 sec), check password |
| **Logs not saving** | Check EEPROM connections, verify WP pin |
| **System resets** | Enable BOD, add decoupling capacitors |

### Quick Test
1. Power on - should show "ENTER PASSWORD"
2. Enter "xyxy" - should go to dashboard
3. Press SW2/SW3 - gear should change
4. Turn pot - speed should change
5. Press SW1 - should show "C "
6. Hold SW4 - should ask for password
7. Enter password - should show menu

## 📊 Log Format

### Memory Map (External EEPROM)
```
Address     Content
0x00-0x0B   Log Entry 0 (HH:MM:SS + event + speed)
0x0C-0x17   Log Entry 1
0x18-0x23   Log Entry 2
...         ...
0x6C-0x77   Log Entry 9
```

### Single Log Entry (12 bytes)
```
Bytes 0-5:   HH:MM:SS (6 bytes)
Bytes 6-7:   Spaces (padding)
Bytes 8-9:   Event (2 bytes)
Bytes 10-11: Speed (2 bytes)
```

## 📋 Technical Specifications

| Parameter | Value |
|-----------|-------|
| Microcontroller | PIC16F877A |
| Clock Frequency | 20 MHz |
| Supply Voltage | 5V DC |
| LCD | 16x2 Character |
| RTC | DS1307 with battery backup |
| EEPROM | 24C04 (512 bytes) |
| Log Capacity | 10 events |
| Password Length | 4 characters |
| Max Attempts | 3 |
| Lockout Time | 2 minutes |
| UART Baud Rate | 9600 |
| Speed Range | 0-100 km/h |
| Gear Positions | ON, GN, G1-G5, GR, C |

## 👨‍💻 Author

**Labhesh Patil**
- GitHub: [@LABHESHP2003](https://github.com/LABHESHP2003)
- Project Link: [https://github.com/LABHESHP2003/Car_Black_Box](https://github.com/LABHESHP2003/Car_Black_Box)

## 📥 Download

### Complete Project Files
Create all files with the content provided in this README.

**File List:**
- `inc/main.h` - Main header
- `inc/config.h` - Configuration
- `inc/adc.h` - ADC header
- `inc/clcd.h` - LCD header
- `inc/dkp.h` - Keypad header
- `inc/ds1307.h` - RTC header
- `inc/ext_eeprom.h` - EEPROM header
- `inc/i2c.h` - I2C header
- `inc/password.h` - Password header
- `inc/uart.h` - UART header
- `src/main.c` - Main program
- `src/login.c` - Login screen
- `src/dashboard.c` - Dashboard
- `src/menu.c` - Menu system
- `src/log.c` - Log manager
- `src/view_log.c` - View logs
- `src/set_time.c` - Set time
- `src/change_password.c` - Change password
- `drivers/adc.c` - ADC driver
- `drivers/clcd.c` - LCD driver
- `drivers/dkp.c` - Keypad driver
- `drivers/ds1307.c` - RTC driver
- `drivers/ext_eeprom.c` - EEPROM driver
- `drivers/i2c.c` - I2C driver
- `drivers/uart.c` - UART driver

Copy each file content from the sections above and save with the corresponding filename.

---

**Made with ❤️ for PIC Genuine Board**