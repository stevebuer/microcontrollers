# PIC Microcontrollers

I have a few of these chips and a K150 programmer.

* [PIC12F675](pic12f675/README.md)
* PIC16x

## Programming

* gputils (Linux) assembler and related utilities
* sdcc (Linux) limited support for PIC platform
* Microchip MPLAB

Configuring SDCC for PIC devices on Debian:

Using cinc2h.pl:

The SDCC project provides a script called cinc2h.pl that can parse Microchip's .inc files (from gputils) and convert them into a .h (header file) format compatible with SDCC.

* sdcc package contains: /usr/share/sdcc/scripts/cinc2h.pl
* Microchip .inc files are in /usr/share/gputils/header/
* download gputils source code
* Use cinc2h.pl script to convert these .inc files into .h files and place them in /usr/share/sdcc/include/pic/

```
% mkdir SRC ; cd SRC ; wget http://deb.debian.org/debian/pool/main/g/gputils/gputils_1.4.0.orig.tar.xz ; unxz gputils* ; tar xvf gputils*
% /usr/share/sdcc/scripts/cinc2h.pl -p p12f675 -I /usr/share/gputils/header/ -gp ~/SRC/gputils-1.4.0.orig
% mv pic14 /usr/share/sdcc/include/pic/
```

Oops! Default Debian sdcc package does not support pic architecture! Need to rebuild!

Install dev tools:

```
% sudo apt update ; sudo apt install build-essential devscripts
```

Download source package

```
apt source sdcc
```

Deps 

```
sudo apt build-dep <package_name>
```

```   
cd <package_name>-<version>
# Make your changes here, e.g., edit files with a text editor
```

```
dch -i
```

```
dpkg-buildpackage -us -uc -b
```

```
sudo dpkg -i ../<package_name>-<version>_<architecture>.deb
```

## References

* [PIC Instructions](https://en.wikipedia.org/wiki/PIC_instruction_listings)
* [Instruction Set](https://technology.niagaracollege.ca/staff/mboldin/18F_Instruction_Set)
* [Register Banking](https://www.microcontrollertips.com/pic-16f-and-register-banking-explained)
* [gputils](https://gputils.sourceforge.io)
* [MPLAB XC8](https://ww1.microchip.com/downloads/en/DeviceDoc/MPLAB%20XC8%20PIC%20Assembler%20User%27s%20Guide%2050002974A.pdf)
* [SDCC](https://sdcc.sourceforge.net) -- PIC support is unmaintained
* [K150 PIC Programmer](https://www.sigmaelectronica.net/manuals/K150.pdf)
* [picpro.py program](https://pypi.org/project/picpro)
* [kitsrus pic programmer](http://www.kitsrus.com/pic.html)

