/* Store data internal eeprom arduino uno (1K)
 * AI generated, Steve modified.
 */

/* 
 *  eeprom write: 3.3ms
 *  eeprom read: ?
 */

/*
 * todo: get/put struct examples
 */

#include <EEPROM.h>

// String myString = "Hello, World!";

int startAddress = 0;

void setup() 
{
  Serial.begin(9600);

  Serial.print("eeprom length: ");
  Serial.println(EEPROM.length());

  /* write once */

  // EEPROM[512] = 77;

  // for (int i = 0; i < myString.length(); i++)
    // EEPROM.write(startAddress + i, myString[i]);
  
  // EEPROM.write(startAddress + myString.length(), '\0');

  char readChar;
  
  String readString = "";
  
  int addr = startAddress;

  while (true) {
    
    readChar = EEPROM.read(addr);
    
    if (readChar == '\0') 
      break;
    
    readString += readChar;
    
    addr++;
  }

  Serial.print("Read from EEPROM: ");
  Serial.println(readString);

  Serial.print("EEPROM[512]: ");
  Serial.println(EEPROM[512]);
}

void loop() {
  // Nothing here
}
