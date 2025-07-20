# ESP8266

32 bit AVR microcontroller with WiFi accessed by AT commands.

## Boards

I have several ESP8266-01 boards.

Espressif Systems <a href="https://en.wikipedia.org/wiki/ESP8266">microcontroller</a> with WiFi.

<img src="esp8266.png">

There is also a USB serial plug-in adapter available for it based on the CH340 chip.

<pre>
Bus 002 Device 005: ID 1a86:7523 QinHeng Electronics CH340 serial converter
</pre>

<img src="esp8266_usbserial.png">

Note: This serial converter doesn't have a way to put the ESP8266 into programming mode. Needs
button to pull GPIO0 to ground. A separate button can pull RST to ground for restart. Need to make a little board
for this. 

More info at [Maker Advisor](https://makeradvisor.com/esp8266-esp-01-usb-serial-programmer)

The pinout is shown below:

<img alt="ESP8266-01 pin diagram" src="https://components101.com/sites/default/files/component_pin/ESP8266-Pinout.png">

## Development Tools

These boards can be programmed with the Arduino IDE by adding: http://arduino.esp8266.com/stable/package_esp8266com_index.json 
in preferences->Additional Board Manager URLs.

## BMP180 weather station

Sketch is partially written to send UDP packets with TEMP/BAROM data -- maybe other stuff? Time?
