/*
 * AI generated serial console template
 * Fixes & mods by Steve
 */

String inputString = "";
bool stringComplete = false;

void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  inputString.reserve(32);
  Serial.println("Command interpreter ready.");
}

void loop() 
{
  if (stringComplete) {
    processCommand(inputString);
    inputString = "";
    stringComplete = false;
  }
}

/* Serial Routines */

/* Note: Runs once per loop() BUT only works on UNO R3, Nano, Mega 2560 Rev3 and Due */

void serialEvent() 
{
  while (Serial.available()) {
  
    char inChar = (char) Serial.read();
    
    if (inChar == '\n')
      stringComplete = true;
    else
      inputString += inChar;
  }
}

void processCommand(String commandLine) 
{
  commandLine.trim();
  
  if (commandLine.length() == 0) 
    return;

  /* Split command and arguments */
  
  int spaceIndex = commandLine.indexOf(' ');
  
  String command = (spaceIndex == -1) ? commandLine : commandLine.substring(0, spaceIndex);
  
  String args = (spaceIndex == -1) ? "" : commandLine.substring(spaceIndex + 1);

  /* match command */
  
  if (command == "led_on") {
    
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("LED turned ON");
 
  } else if (command == "led_off") {
    
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("LED turned OFF");
  
  } else if (command == "echo")  
      Serial.println("Echo: " + args);
    else
      Serial.println("Unknown command: " + command);
}
