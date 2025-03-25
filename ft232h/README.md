# Adafruit FT232H Breakout Board

My device is an older FT232H with a micro USB connector.

<img src="ft232h.png">

```
Bus 002 Device 005: ID 0403:6014 Future Technology Devices International, Ltd FT232H Single HS USB-UART/FIFO IC
```

## Adafruit Blinka library setup

The following is the setup process on Debian Bookworm.

1. Install pyftdi library, one which Blinka depends.

```
sudo apt install python3-ftdi
```

2. Add the relevant lines to /etc/udev/rules.d/11-ftdi.rules depnding on your device.

```
SUBSYSTEM=="usb", ATTR{idVendor}=="0403", ATTR{idProduct}=="6001", GROUP="plugdev", MODE="0666"
SUBSYSTEM=="usb", ATTR{idVendor}=="0403", ATTR{idProduct}=="6011", GROUP="plugdev", MODE="0666"
SUBSYSTEM=="usb", ATTR{idVendor}=="0403", ATTR{idProduct}=="6010", GROUP="plugdev", MODE="0666"
SUBSYSTEM=="usb", ATTR{idVendor}=="0403", ATTR{idProduct}=="6014", GROUP="plugdev", MODE="0666"
SUBSYSTEM=="usb", ATTR{idVendor}=="0403", ATTR{idProduct}=="6015", GROUP="plugdev", MODE="0666"
```

3. Install blinka in a Pyton virtual environment. Bookworm does not seem to allow pip installs. Is blinka available in an apt repository?
```
python -m venv mydir
mydir/bin/pip3 install blinka
```

4. Export shell variable

```
export BLINKA_FT232H=1

```

5. Wire circuit with bus lines and pullup resistors
