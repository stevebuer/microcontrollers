/* keypad example
 *  
 */

#include <Key.h>
#include <Keypad.h>

const byte ROWS = 4; 
const byte COLS = 3;

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

/* oops -- can't use pin 13 -- it has led on it! */

byte rowPins[ROWS] = {6, 7, 8, 9}; 
byte colPins[COLS] = {10, 11, 12};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup()
{
  Serial.begin(9600);
}
  
void loop()
{
  char key = keypad.getKey();
  
  if (key)
    Serial.println(key);
}
