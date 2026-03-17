/*
 * UART Communication Example for 8051 / AT89S52 Microcontrollers
 *
 * Brief Explanation:
 * This program demonstrates basic Universal Asynchronous Receiver-Transmitter (UART)
 * serial communication. It is configured for a baud rate of 9600 bps using Timer 1
 * in Mode 2 (8-bit auto-reload). The microcontroller receives a character via the
 * RXD pin and echoes the exact same character back via the TXD pin.
 *
 * Hardware Assumptions:
 * - Oscillator Frequency: 11.0592 MHz (standard for generating accurate baud rates)
 */

#include <reg51.h> // Standard 8051 register definitions

// Function prototypes
void UART_Init(void);
void UART_Transmit(char data_byte);
char UART_Receive(void);

void main(void) {
    char received_char;

    // Initialize UART communication
    UART_Init();

    // Send an initial test message
    UART_Transmit('O');
    UART_Transmit('K');
    UART_Transmit('\r'); // Carriage return
    UART_Transmit('\n'); // Line feed

    // Infinite loop to continuously receive and echo data
    while(1) {
        received_char = UART_Receive(); // Wait for an incoming character
        UART_Transmit(received_char);   // Send the character back (echo)
    }
}

// Initialize UART for 9600 baud rate
void UART_Init(void) {
    TMOD = 0x20; // Timer 1, Mode 2 (8-bit auto-reload)
    TH1 = 0xFD;  // Load value for 9600 baud rate (assuming 11.0592 MHz crystal)
    SCON = 0x50; // Serial Mode 1 (8-bit data, 1 stop bit), Enable Receiver (REN=1)
    TR1 = 1;     // Start Timer 1
}

// Transmit a single character over UART
void UART_Transmit(char data_byte) {
    SBUF = data_byte; // Load data into the Serial Buffer register
    while(TI == 0);   // Wait until Transmit Interrupt flag is set (transmission complete)
    TI = 0;           // Clear Transmit Interrupt flag for the next byte
}

// Receive a single character over UART
char UART_Receive(void) {
    while(RI == 0);   // Wait until Receive Interrupt flag is set (data received)
    RI = 0;           // Clear Receive Interrupt flag for the next byte
    return SBUF;      // Return the data from the Serial Buffer register
}
