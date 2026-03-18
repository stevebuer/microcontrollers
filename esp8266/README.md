# ESP8266

Espressif Systems L106 32 bit microcontroller with WiFi accessed by AT commands.

Can be programmed with:

* Arduino
* Non-OS SDK
* RTOS SDK

## Boards

### NodeMCU

<img src="img/nodemcu_clone.jpg">

Arduino board support.

<img width="400" src="https://mischianti.org/wp-content/uploads/2021/10/NodeMcu-V3-CH340-Lua-ESP8266-pinout-mischianti-high-resolution.png" alt="NodeMCU v3 Pinout">

### ESP-01 Modules

I have several ESP8266-01 boards.

Espressif Systems <a href="https://en.wikipedia.org/wiki/ESP8266">microcontroller</a> with WiFi.

<img src="img/esp8266.jpg" width="300">

There is also a USB serial plug-in adapter available for it based on the CH340 chip.

<pre>
Bus 002 Device 005: ID 1a86:7523 QinHeng Electronics CH340 serial converter
</pre>

<img src="img/esp8266_usbserial.png" width="200">

Note: This serial converter doesn't have a way to put the ESP8266 into programming mode. Needs
button to pull GPIO0 to ground. A separate button can pull RST to ground for restart. Need to make a little board
for this. 

More info at [Maker Advisor](https://makeradvisor.com/esp8266-esp-01-usb-serial-programmer)

The pinout is shown below:

<img alt="ESP8266-01 pin diagram" src="https://components101.com/sites/default/files/component_pin/ESP8266-Pinout.png">

## Development Tools

* Arduino IDE with correct board support package.
* Platform IO

## Weather Station Project

Todo: bmp180, humidity sensor on nodemcu to MQTT.

