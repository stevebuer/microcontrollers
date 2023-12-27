// Send CQ on the Builtin LED for testing

#include <cww_MorseTx.h>

#define CW_SPEED 10

cww_MorseTx morse(LED_BUILTIN, CW_SPEED);

void setup() {

  Serial.begin(9600);
}

void loop() {

    Serial.println("Sending CQ");

    morse.send("CQ DE N7MKO");

    // morse.send("V K V K 73");

    delay(5000);
}

// Upload Notes

// Red Mini: Arduino Pro/Mini 328P 3.3V, 8Mhz
// Blue Mini: Arduino Pro/Mini 328P 5V, 16Mhz

// Adafruit USB Serial: Blk=GND, RED=5V, Green=USB TX out, White= USB RX in
