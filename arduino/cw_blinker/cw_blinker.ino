/*
 *  Send Morse Beacon on Builtin LED for testing
 */

#include <cww_MorseTx.h>

#define CW_SPEED 10

cww_MorseTx morse(LED_BUILTIN, CW_SPEED);

void setup() 
{
  Serial.begin(9600);
}

void loop() 
{
    Serial.println("N7MKO BEACON");
    morse.send("vvv de n7mko n7mko n7mko");
    delay(5000);
}
