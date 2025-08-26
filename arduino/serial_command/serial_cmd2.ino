/*
 * AI generated
 */

String inputString = "";     // Stores incoming serial data

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("Command interpreter ready.");
}

void loop() {
  // Read incoming characters while available
  while (Serial.available() > 0) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      processCommand(inputString);
      inputString = ""; // Clear buffer after processing
    } else {
      inputString += inChar;
    }
  }
}

void processCommand(String commandLine) {
  commandLine.trim(); // Remove leading/trailing whitespace
  if (commandLine.length() == 0) return;

  // Split command and arguments
  int spaceIndex = commandLine.indexOf(' ');
  String command = (spaceIndex == -1) ? commandLine : commandLine.substring(0, spaceIndex);
  String args = (spaceIndex == -1) ? "" : commandLine.substring(spaceIndex + 1);

  // Match and execute command
  if (command == "led_on") {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("LED turned ON");
  } else if (command == "led_off") {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("LED turned OFF");
  } else if (command == "echo") {
    Serial.println("Echo: " + args);
  } else {
    Serial.println("Unknown command: " + command);
  }
}
