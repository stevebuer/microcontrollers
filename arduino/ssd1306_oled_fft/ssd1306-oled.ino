#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <arduinoFFT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_MOSI 11
#define OLED_CLK  13
#define OLED_DC   8
#define OLED_CS   10
#define OLED_RESET 9

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#define SAMPLES 128

double vReal[SAMPLES];
double vImag[SAMPLES];

ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, SAMPLES, SAMPLING_FREQUENCY);

void setup() 
{
	display.begin(SSD1306_SWITCHCAPVCC);
	display.setRotation(0);
}

void loop() 
{
  // 1. sample analog input into vReal[], zero vImag[]
  // 2. FFT.windowing(...), FFT.compute(...), FFT.complexToMagnitude(...)

	display.clearDisplay();

	int bars = SCREEN_WIDTH / 2; // e.g. 64 bars, 2px wide

	for (int i = 0; i < bars; i++) {

		int barHeight = map(vReal[i], 0, 4095, 0, SCREEN_HEIGHT); // scale to your FFT output range
		display.fillRect(i * 2, SCREEN_HEIGHT - barHeight, 2, barHeight, SSD1306_WHITE);
	}

	display.display();
}
