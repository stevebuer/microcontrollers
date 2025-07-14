#!/bin/python3
# 
# FT232H PyFTDI GPIO Example
# Adafruit rev1 board
# Steve Buer
#

import time
from pyftdi.gpio import GpioAsyncController

LED_ON = 1
LED_OFF = 0

SECONDS = 1

print('FT232H PYFTDI GPIO example')

# PYFTDI GPIO library uses AD0-8 port for GPIO only?

gpio = GpioAsyncController()

gpio.configure('ftdi:///1', direction=0xFF, initial=0x0)

port = gpio.get_gpio()

# loop

while True:
	port.write(LED_ON)
	time.sleep(SECONDS)
	port.write(LED_OFF)
	time.sleep(SECONDS)
