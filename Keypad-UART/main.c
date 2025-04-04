#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"

// Keypad layout (4x4)
const char keypad[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// Pin definitions
#define ROW_PORT GPIO_PORTE_BASE
#define COL_PORT GPIO_PORTD_BASE
#define ROW_PINS (GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4)
#define COL_PINS (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)

#define GPIO_PORTB_BASE 0x40005000
#define GPIO_PORTD_BASE 0x40007000
#define GPIO_PORTE_BASE 0x40024000
#define GPIO_PORTA_BASE 0x40004000
#define UART0_BASE 0x4000C000

// Function prototypes
void UART0_Init(void);
void Keypad_Init(void);
char Keypad_GetKey(void);
void UART_SendChar(char c);
void UART_SendString(const char *str);

int main(void) {
    // Set system clock to 80 MHz
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // Initialize UART0 and Keypad
    UART0_Init();
    Keypad_Init();

    UART_SendString("Keypad Test: Press a key\r\n");

    while (1) {
        char key = Keypad_GetKey();
        if (key != 0) { // Key pressed
            UART_SendString("Key pressed: ");
            UART_SendChar(key);
            UART_SendString("\r\n");
            SysCtlDelay(8000000); // Debounce delay (~100ms)
        }
    }
}

void UART0_Init(void) {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0) || !SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    UARTEnable(UART0_BASE);
}

void Keypad_Init(void) {
    // Enable GPIO Ports E and D
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE) || !SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD));

    // Rows as outputs
    GPIOPinTypeGPIOOutput(ROW_PORT, ROW_PINS);
    GPIOPinWrite(ROW_PORT, ROW_PINS, 0x00); // All rows low initially

    // Columns as inputs with pull-downs
    GPIOPinTypeGPIOInput(COL_PORT, COL_PINS);
    GPIOPadConfigSet(COL_PORT, COL_PINS, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD); // Weak pull-down
}

char Keypad_GetKey(void) {
    uint8_t row, col;

    // Scan each row
    for (row = 0; row < 4; row++) {
        // Set one row high, others low
        GPIOPinWrite(ROW_PORT, ROW_PINS, (1 << (row + 1)));

        // Read columns
        uint8_t col_state = GPIOPinRead(COL_PORT, COL_PINS);
        if (col_state) {
            // Find which column is high
            for (col = 0; col < 4; col++) {
                if (col_state & (1 << col)) {
                    GPIOPinWrite(ROW_PORT, ROW_PINS, 0x00); // Reset rows
                    return keypad[row][col];
                }
            }
        }
    }
    GPIOPinWrite(ROW_PORT, ROW_PINS, 0x00); // Reset rows
    return 0; // No key pressed
}

void UART_SendChar(char c) {
    UARTCharPut(UART0_BASE, c);
}

void UART_SendString(const char *str) {
    while (*str) {
        UARTCharPut(UART0_BASE, *str++);
    }
}
