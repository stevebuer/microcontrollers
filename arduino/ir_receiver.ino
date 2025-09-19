/* 
 *  IR Remote Example
 */

#include <IRremote.hpp>

const int RECV_PIN = 2;

// IRrecv irrecv(RECV_PIN);

decode_results results;

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);
}

void loop() {
  if (IrReceiver.decode()) {
    Serial.print("Received IR code: ");
    IrReceiver.printIRResultMinimal(&Serial);
    Serial.println();
    IrReceiver.resume();
  }
}
