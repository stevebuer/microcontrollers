#!mydir/bin/python3

#
# Control Adafruit 4 digit 7 segment LED display via i2c
#

# bash% export BLINKA_FT232H=1

import sys
import time
import board
import busio

from adafruit_ht16k33 import segments # class for 7x4 display
from time import sleep

i = busio.I2C(board.SCL, board.SDA) 

d = segments.Seg7x4(i)

d.brightness = 0.25 # brightness

d.fill(0) # 0 blanks (clears) the display, any other values turns all segments on 

d.print(1111) # print digits, shifting any remaining digits to the left on display
sleep(2)
d.print('00')
sleep(2)
d.print('cd')
sleep(2)

# set 1 character at a time via array access

d.fill(0)
d[0] = 'b'
d[1] = 'e'
d[2] = 'e'
d[3] = 'f'
sleep(2)
d[3] = 'r'
sleep(2)

# print floats as strings

d.fill(0)
d.print('20.25')
sleep(2)

d.fill(0)

i = 123.5

d.print(str(i))
sleep(2)

# can interpret colon and dot

d.fill(0)
d.print(":")
d.print(".")
sleep(2)
d.print("...")
d.colon = False
d.print("done")

# other stuff tested in REPL:
#
# d.colon = True
# d.blink_rate = 1 (or 2 or 3 with 0 off)
# d.brightness = 0.1
# d.show() when d.auto_write = False
# d.marquee('hello world', loop=False)
# d.set_digit_raw(0, 13) <-- bit pattern to use
# s.scroll(1) <-- does nothing?
