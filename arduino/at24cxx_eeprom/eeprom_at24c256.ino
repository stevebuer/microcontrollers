/* 
 * EEPROM Read/Write
 * Tested with AT24C256 256k module and AT24C32 32k on DS1307 module.
 * Copilot & Steve
 */
 
#include <Wire.h>

#define EEPROM_ADDR 0x50  // AT24C256 I2C address

const byte bmpData[] = {
  0x42, 0x4D, 0x3E, 0x00, 0x00, 0x00, // BMP header (partial)
  0x00, 0x00, 0x00, 0x00, 0x36, 0x00,
  // Pixel data (dummy values)
  0xFF, 0x00, 0xAA, 0x55, 0xFF, 0x00, 0xAA, 0x55
};

const char message[] = "EEPROM Storage Test";

char buf[32];

void writeEEPROM(unsigned int addr, const byte* data, int len) 
{
  int i = 0;
  while (i < len) {
    Wire.beginTransmission(EEPROM_ADDR);
    Wire.write((int)(addr >> 8));     // MSB
    Wire.write((int)(addr & 0xFF));   // LSB

    // Write up to 16 bytes per page
    for (int j = 0; j < 16 && i < len; j++, i++) {
      Wire.write(data[i]);
    }

    Wire.endTransmission();
    delay(10); // EEPROM write delay
    addr += 16;
  }
}

// Reads 'len' bytes starting from EEPROM address 'addr' into 'buffer'
void readEEPROM(unsigned int addr, byte* buffer, int len) {
  int i = 0;

  while (i < len) {
    Wire.beginTransmission(EEPROM_ADDR);
    Wire.write((int)(addr >> 8));     // MSB of memory address
    Wire.write((int)(addr & 0xFF));   // LSB of memory address
    Wire.endTransmission();

    // Request up to 32 bytes at a time (Wire library limit)
    int chunk = min(len - i, 32);
    Wire.requestFrom(EEPROM_ADDR, chunk);

    int j = 0;
    while (Wire.available() && j < chunk) {
      buffer[i++] = Wire.read();
      j++;
    }

    addr += chunk;
  }
}

void setup() {
  Wire.begin();
  Serial.begin(9600);
  //Serial.println("Writing BMP to EEPROM...");
  //writeEEPROM(0x0000, bmpData, sizeof(bmpData));
  //writeEEPROM(0x00, message, sizeof(message));
  //Serial.println("Done!");
}

void loop() {

  memset(buf, 0, sizeof(buf));
  readEEPROM(0x0000, buf, sizeof(buf));
  Serial.print("buf: ");
  Serial.println(buf);
  delay(10000);
}
