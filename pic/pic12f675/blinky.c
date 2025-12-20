/*
 * PIC12F675 blinky for sdcc
 */

#include <pic/pic12f675.h>

// Configuration word for PIC12F675
unsigned int at 0x2007 __CONFIG = 0x31C4;

void main(void) {
    // Set all GPIO pins as output
    TRISIO = 0x00;

    // Main loop
    while (1) {
        // Turn on LED connected to GP0
        GPIO = 0x01;

        // Simple delay loop
        for (unsigned int i = 0; i < 10000; i++);

        // Turn off LED
        GPIO = 0x00;

        // Delay again
        for (unsigned int i = 0; i < 10000; i++);
    }
}

