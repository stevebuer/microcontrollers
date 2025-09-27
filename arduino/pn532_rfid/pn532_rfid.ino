/*
 * PN532 RFID Module
 *
 * DIP switches: 1,0
 */

#include <Wire.h>
#include <Adafruit_PN532.h>

#define SDA_PIN A4
#define SCL_PIN A5

Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);

void setup() 
{
  Serial.begin(9600);
  Serial.println("PN532 RFID/NFC Test");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("PN532 not found");
    while (1);
  }

  Serial.println("Found PN532 module.");
  nfc.SAMConfig(); 
  Serial.println("Waiting for an NFC card...");
}

void loop() 
{
  uint8_t success;
  uint8_t uid[] = { 0 };
  uint8_t uidLength;

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    Serial.println("Card Found");
    Serial.print("UID Length: ");
    Serial.print(uidLength, DEC);
    Serial.println(" bytes");
    Serial.print("UID Value: ");
    
    for (uint8_t i = 0; i < uidLength; i++) {
      Serial.print(" 0x");
      Serial.print(uid[i], HEX);
    }
    
    Serial.println("");
    delay(1000);
  }
}
