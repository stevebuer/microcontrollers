#!mydir/bin/python3
#
# run in virtualenv mydir
# 
# FT232H Blinka GPIO Example
# Steve Buer
#

import time
import board
import digitalio

pin = board.C0
direction = digitalio.Direction.OUTPUT
seconds = 1

print('FT232H GPIO example')

led = digitalio.DigitalInOut(pin)

led.direction = direction

while True:
    led.value = True
    time.sleep(seconds)
    led.value = False
    time.sleep(seconds)
