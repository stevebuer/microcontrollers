#!/usr/bin/python3

# 
# Check BMP180 device is connected using pyftdi
# Steve Buer, N7MKO
# 

import sys
import pyftdi 

from pyftdi.i2c import I2cController

FTDI_DEVICE='ftdi://ftdi:232h/1'
BMP180_I2CADDR=0x77
BMP180_ID_REGISTER=0xD0

print("opening device:", FTDI_DEVICE)

try:

    i2c = I2cController()

    i2c.configure(FTDI_DEVICE)

except pyftdi.usbtools.UsbToolsError as e:

    print("error:", e)
    sys.exit(1)

slave = i2c.get_port(BMP180_I2CADDR)

print("checking BMP180 register: %x at address: %x" % (BMP180_ID_REGISTER, BMP180_I2CADDR))

try:

    slave.write([0xD0])

    result = slave.exchange([0xD0], 1)

except pyftdi.i2c.I2cNackError as e:

    print("error:", e)
    sys.exit(2)

print("read value:", result.hex())
