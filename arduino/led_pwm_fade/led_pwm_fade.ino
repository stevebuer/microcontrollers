/* 
 *  Arduino Nano: fade red LED via 560 ohm resistor 
 */

int ledPin = 9;        // LED connected to digital pin 9 
int brightness = 0;    // Initial brightness
int fadeAmount = 5;    // Amount to change brightness

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  analogWrite(ledPin, brightness);
  brightness += fadeAmount;

  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount; // Reverse direction
  }

  delay(50); // Controls speed of fading
}
