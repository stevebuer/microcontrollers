/*
 * libFTDI test and blink GPIO
 * Steve Buer
 */
 
#include <stdio.h>
#include <libftdi1/ftdi.h>

#define USB_VENDOR 0x0403
#define USB_DEVICE 0x6014
 
int main(int argc, char **argv)
{
	int r;
	unsigned int chipid;
	struct ftdi_context *ftdi;

	ftdi = ftdi_new();

	if (ftdi == 0) {

		fprintf(stderr, "ftdi_new() failed\n");
		return 1;
	}

	r = ftdi_usb_open(ftdi, USB_VENDOR, USB_DEVICE);
 
	if (r < 0) {
	
		fprintf(stderr, "ftdi_usb_open() failed: (%s)\n", ftdi_get_error_string(ftdi));
        	return 2;
	}

	// Test connectivity, get chip ID

	if (ftdi->type == TYPE_232H) {

		r = ftdi_read_chipid(ftdi, &chipid);

		if (r < 0) {

			fprintf(stderr, "ftdi_read_chip() failed: %d (%s)\n", r, ftdi_get_error_string(ftdi));
			return 3;
		} 

		printf("FTDI chipid: %X\n", chipid);

	} else {

		printf("unknown chip type\n");
		return 4;
	}

	// todo: do some gpio on LEDS

	/* close connection and exit */
 
	if (ftdi_usb_close(ftdi) < 0) {
        
		fprintf(stderr, "unable to close ftdi device: (%s)\n", ftdi_get_error_string(ftdi));
        	ftdi_free(ftdi);
        	return 5;
	}
 
	return 0;
}
