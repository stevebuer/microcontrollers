
#include <TEA5767Radio.h>
// #include <GhostBox.h>

TEA5767Radio radio = TEA5767Radio();

float freq = 88.0;
float f_max = 108.0;

int pause_time = 0; // additionally millis to pause (library does 100ms by default)

void setup()
{ 
  Serial.begin(9600);
  Wire.begin();
}

void ghostbox_scan()
{
  for (float f_cur = 88.1; f_cur < f_max; f_cur += 0.2) {

    radio.setFrequency(f_cur);

    if (pause_time > 0)
      delay(pause_time);
  } 
}

void loop()
{
  if (Serial.available())
    ghostbox_scan();
}
