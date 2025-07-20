/*
 * Send test message on built-in LED
 * Steve Buer, N7MKO
 */

#include <cww_MorseTx.h>

#define CW_SPEED 10

cww_MorseTx morse(LED_BUILTIN, CW_SPEED);

void setup() {

  Serial.begin(9600);
}

void loop() {

    Serial.println("Sending CW test...");

    morse.send("V V V DE N7MKO N7MKO N7MKO");

    delay(2000);
}

// Device Notes:
//
// Red Mini: Arduino Pro/Mini 328P 3.3V, 8Mhz
// Blue Mini: Arduino Pro/Mini 328P 5V, 16Mhz
//
// Adafruit USB Serial: Blk=GND, RED=5V, Green=USB TX out, White=USB RX in
