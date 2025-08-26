/* 
 *  AI generated millis example
 *  Steve mods
 */

unsigned long previousMillis = 0;
const long interval = 1000;
int myNumber = 0;
const int median = 68;

void setup()
{
  Serial.begin(9600);
}

void loop() 
{
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    
    previousMillis = currentMillis;

    Serial.print("Voltage:");
    Serial.print(myNumber);
    Serial.print(", Threshold:");
    Serial.println(median);

    myNumber = random(100);
    
  }

  /* other loop code */
}
