/* 
 * AI Generated 
 * Debugged / modded by Steve Buer
 */

/* Use monitor settings: 'No line ending' */

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("CMD1 Ready for single-character commands:");
  Serial.println("l = LED ON, o = LED OFF, e = Echo mode");
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();

    // Serial.print("Debug Received: ");
    // Serial.println(command);  // This will show invisible characters like \r or \n

    /* compiler bug? Declare var inside switch() breaks cases below it! */
    /* Move this declaration here */
    String echoText;

    switch (command) {
      case 'c':
        Serial.println("C command");
        break;
      
      case 'l': // LED ON
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.println("LED turned ON");
        break;

      case 'o': // LED OFF
        digitalWrite(LED_BUILTIN, LOW);
        Serial.println("LED turned OFF");
        break;

      case 'e':
        Serial.println("Enter text to echo:");
        while (Serial.available() == 0); // Wait for input
        echoText = Serial.readStringUntil('\n');
        echoText.trim();
        Serial.println("Echo: " + echoText);
        break;

      case 'h':
      default:
        Serial.print("Unknown command: ");
        Serial.println(command);
        break;
    }
  }
}
