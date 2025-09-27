/*
 * One Wire Scan
 * 
 */
#include <OneWire.h>

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);

void setup() {
  Serial.begin(9600);
  Serial.println("Scanning 1-Wire bus...");
}

void loop() {
  byte addr[8];

  if (!oneWire.search(addr)) {
    Serial.println("No more devices found.");
    oneWire.reset_search();
    delay(2500);
    return;
  }

  Serial.print("Found device with ROM: ");
  for (int i = 0; i < 8; i++) {
    if (addr[i] < 16) Serial.print("0");
    Serial.print(addr[i], HEX);
    Serial.print(" ");
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
    Serial.println("\nCRC is not valid!");
  } else {
    Serial.println("\nCRC is valid.");
  }

  delay(1000);
}
