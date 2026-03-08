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
