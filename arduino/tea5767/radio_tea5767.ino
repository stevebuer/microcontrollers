/* 
 *  TEA5767 via Radio library
 *  Spirit Box Scanner
 *  Steve Buer
 */

#include <TEA5767.h>

RADIO_FREQ frequency = 8800;

TEA5767 radio;

int extra_delay = 500;

void setup() {

  Serial.begin(9600);

  radio.init();
  radio.setBand(RADIO_BAND_FM);
  radio.setMono(true);
  radio.setMute(true);
}

void SpiritScan()
{
  radio.setMute(false);
  
  for (frequency = 8810; frequency < 10800; frequency += 20) {

    radio.setFrequency(frequency);

    // radio.getFrequency(); /* verify */

    if (extra_delay > 0)
      delay(extra_delay);
  }

  radio.setMute(true);
}

void loop() { 

    if (Serial.available()) {

      char c = Serial.read();

      if (c != 's')
        return;
        
      Serial.println("SCAN");
      SpiritScan();
      Serial.println("END SCAN");
    }
}
