/*
 * Does it blink?
 */

void setup() 
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() 
{
  Serial.write("blink ");
  digitalWrite(LED_BUILTIN, HIGH);   
  delay(250);  
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}
