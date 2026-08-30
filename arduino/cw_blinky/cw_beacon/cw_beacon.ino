/*
 *  Send Morse Beacon on Builtin LED for testing
 */

#include <cww_MorseTx.h>

#define CW_SPEED 13

#define CW_MESSAGE "vvv vvv vvv de n7mko n7mko n7mko"

cww_MorseTx morse(LED_BUILTIN, CW_SPEED);

void setup() 
{
  Serial.begin(9600);
}

void loop() 
{
    Serial.println(CW_MESSAGE);
    morse.send(CW_MESSAGE);
    delay(2500);
}
