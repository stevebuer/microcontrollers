#
# BMP180 via FTDI FT232H using pyftdi
#
#

FTDI=ftdi://ftdi:232h:1/1

all:

	@./bmp180-check.py
 
# list devices and scan i2c bus with pyftdi
 
scan:

	@ ftdi_urls.py
	@ i2cscan.py $(FTDI)
