# Keypad and UART Interfacing with TIVA C LaunchPad

## Overview
This project demonstrates interfacing a 4x4 keypad and UART communication with the TIVA C LaunchPad (EK-TM4C123GXL, TM4C123GH6PM microcontroller). The keypad captures user input (e.g., numbers, commands), which is then transmitted via UART to a PC terminal (e.g., PuTTY, Tera Term) for display or further processing. The project showcases embedded systems skills, including GPIO configuration, UART setup, and keypad scanning.

### Features
- Interfaces a 4x4 keypad to capture user input (0-9, A-D, *, #).
- Transmits keypad input to a PC terminal via UART at 115200 baud rate.
- Implements interrupt-driven keypad scanning for efficient input detection.
- Configures the TIVA C LaunchPad’s system clock to 16 MHz for accurate timing.
- Includes UART-based debugging for real-time feedback.

## Hardware Requirements
- **TIVA C LaunchPad**: EK-TM4C123GXL with TM4C123GH6PM microcontroller.
- **4x4 Keypad**: Standard matrix keypad with 16 keys.
- **Jumper Wires**: For connecting the keypad to the LaunchPad.
- **USB-to-Serial Connection**: The LaunchPad’s USB interface (via ICDI) for UART communication with a PC.
- **Connections**:
  - Keypad Rows (R1-R4) → TIVA GPIO (e.g., PE1-PE4)
  - Keypad Columns (C1-C4) → TIVA GPIO (e.g., PD0-PD3)
  - UART0 TX (PA1) → Used for serial communication (via LaunchPad USB to PC)
  - UART0 RX (PA0) → Optional (for receiving commands from PC)

## Software Requirements
- **Code Composer Studio (CCS)**: Version 12.4.0 or later.
- **TivaWare**: Version 2.2.0.295 or later.
- **Compiler**: TI ARM Compiler (included with CCS).
- **Terminal Software**: PuTTY, Tera Term, or CCS Terminal for viewing UART output.

## Setup Instructions
1. **Hardware Setup**:
   - Connect the 4x4 keypad to the TIVA C LaunchPad as per the pin assignments above.
   - Connect the LaunchPad to your PC via USB (this also enables UART communication).
2. **Software Setup**:
   - Clone or download this repository.
