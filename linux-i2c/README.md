# Linux i2C Support

Packages:

   * i2c-tools
   * libi2c-dev
   * python-smbus

## Linux Device Tree

Open Firmware Device Tree (Device Tree): data structure and language for describing hardware

[Kernel Device Tree Documentation](https://www.kernel.org/doc/html/latest/devicetree/index.html)

[Device Tree Reference](https://elinux.org/Device_Tree_Reference)

.dtb: device tree blob
.dtbo: device tree overlay

/boot/config.txt: RPi boot loader config

More info on booting and [Bare Metal Raspbery Pi Development by David Welch](https://github.com/dwelch67/raspberrypi)

### Raspberry Pi Zero W

Todo: Enable i2c hardware support in /boot/config.txt

   * Software i2c bus via GPIO
   * Hardware i2c support
