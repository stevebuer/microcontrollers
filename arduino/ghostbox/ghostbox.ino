// Ghostbox using TEA5767 chip & Arduino Radio library
// Steve, N7MKO

#include <Button.h>
#include <TEA5767.h>

#define SWEEP_BUTTON_PIN 2

#define IDLE 0
#define SCANNING 1

int state = IDLE;

RADIO_FREQ frequency = 8800;

TEA5767 radio;

Button sweep_button(SWEEP_BUTTON_PIN);

void setup() {

  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  sweep_button.begin();

  radio.init();

  radio.setBand(RADIO_BAND_FM);

  radio.setMono(true);

  radio.setMute(true); // test?

  // set initial state to mute
}

void loop() {

    if (state == IDLE) {
      
      if (sweep_button.pressed()) {

         Serial.println("Begin Sweep");
         state = SCANNING;
         radio.setMute(false);
      }
      
      return;
    }

    Serial.print("FREQ ");
    Serial.println(frequency);
    
    radio.setFrequency(frequency);
    
    delay(60);
    
    frequency += 10;

    if (frequency >= 10800) {

      delay(100);
      radio.setMute(true);
      delay(100);
      
      frequency = 8800;
      state = IDLE;
      Serial.println("Sweep End");
    }

}
