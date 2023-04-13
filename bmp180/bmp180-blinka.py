#!/usr/bin/python3

#
# Test BMP180 via Adafruit Blinka library

import sys
import board
import busio
from busio import I2C

BMP180_ADDR = 0x77
ID_REGISTER = 0xD0

# class

class BMP180():

    def __init__(self):

        device = 'test'
        self.i2c = busio.I2C(board.SCL, board.SDA)

        if BMP180_ADDR in self.i2c.scan():
            print("Found BMP180 at:", hex(BMP180_ADDR))
        else:
            print("BMP180 not found")

    def id_register(self):
            
        print("Querying BMP180 ID Register")

        result = bytearray(1)

        self.i2c.writeto(BMP180_ADDR, bytes([0xD0]))

        self.i2c.readfrom_into(BMP180_ADDR, result)

        print("Result:", result.hex())

# main 

sensor = BMP180()

sensor.id_register()

