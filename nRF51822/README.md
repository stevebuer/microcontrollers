# Nordic Semi nRF51822

Nordic Semiconductor [nRF51822](https://www.mouser.com/datasheet/2/297/nRF51822_PB_v2.5-1108907.pdf?srsltid=AfmBOooAkWt8JKc27uyOOnIDmTojK6vN8Y4nUoq-lGm-uYNzx4Eoab2s) BLE SoC.

Plugs in to Waveshare [BLE4.0 mother board](https://docs.nordicsemi.com/bundle/ncs-3.2.4/page/zephyr/boards/waveshare/nrf51_ble400/doc/index.html) / development board.

## BLE400 Board

* Does not include debugger!
* USB Port is UART only!
* Input: Two user buttons
* Output: 4 LEDs

## Development Tools

* nRF5 SDK 12.3.0 (This is an older chip)
* nRF Connect for Desktop
	1. Programmer → Flash hex files (with J‑Link)
	2. Bluetooth Low Energy → Test GATT services
	3. Power Profiler (optional)
* PlatformIO?

## Debug / Programming Adapter

* Segger J-LINK (preferred)
* ST-LINK V2 (openocd support?)
* Bus Pirate
