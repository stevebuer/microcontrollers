#include "stm8s_conf.h"
#include "onewire.h"
#include "uart.h"

#define ONEWIRE_PORT GPIOD
#define ONEWIRE_PIN GPIO_PIN_2

static void ow_delay_us(unsigned int us)
{
	uint8_t chunk;

	while (us) {
		chunk = us > 250U ? 250U : (uint8_t)us;
		TIM2->CNTRL = 0;
		while (TIM2->CNTRL < chunk) {
		}
		us -= chunk;
	}
}

static void ow_drive_low(void)
{
	GPIO_WriteLow(ONEWIRE_PORT, ONEWIRE_PIN);
}

static void ow_drive_high(void)
{
	GPIO_WriteHigh(ONEWIRE_PORT, ONEWIRE_PIN);
}

static unsigned char ow_read_bit(void)
{
	ow_drive_low();
	ow_delay_us(2);
	ow_drive_high();
	ow_delay_us(10);
	return (GPIO_ReadInputPin(ONEWIRE_PORT, ONEWIRE_PIN) == RESET) ? 0 : 1;
}

static unsigned char ow_read_byte(void)
{
	unsigned char byte = 0;
	unsigned char bit;
	for (bit = 0; bit < 8; bit++) {
		byte >>= 1;
		if (ow_read_bit()) {
			byte |= 0x80;
		}
		ow_delay_us(45);
	}
	return byte;
}

static void ow_write_bit(unsigned char bit)
{
	ow_drive_low();
	if (bit) {
		ow_delay_us(6);
	} else {
		ow_delay_us(60);
	}
	ow_drive_high();
	if (bit) {
		ow_delay_us(54);
	} else {
		ow_delay_us(10);
	}
}

static void ow_write_byte(unsigned char byte)
{
	unsigned char bit;
	for (bit = 0; bit < 8; bit++) {
		ow_write_bit((byte >> bit) & 0x01);
	}
}

static unsigned char ow_reset(void)
{
	ow_drive_low();
	ow_delay_us(480);
	ow_drive_high();
	ow_delay_us(70);
	if (GPIO_ReadInputPin(ONEWIRE_PORT, ONEWIRE_PIN) == RESET) {
		ow_delay_us(410);
		return 1;
	}
	ow_delay_us(410);
	return 0;
}

static void ow_print_hex_byte(unsigned char value)
{
	uart_puts("0x");
	uart_puthex8(value);
}

void onewire_init(void)
{
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, ENABLE);
	TIM2->PSCR = 4;
	TIM2->ARRH = 0;
	TIM2->ARRL = 0xFF;
	TIM2->CNTRH = 0;
	TIM2->CNTRL = 0;
	TIM2->CR1 = TIM2_CR1_CEN;

	GPIO_Init(ONEWIRE_PORT, ONEWIRE_PIN, GPIO_MODE_OUT_OD_HIZ_FAST);
	GPIO_ExternalPullUpConfig(ONEWIRE_PORT, ONEWIRE_PIN, ENABLE);
	ow_drive_high();
}

void onewire_handle_command(char* args)
{
	if (!args || *args == '\0') {
		uart_puts("1-wire commands:\r\n");
		uart_puts("  ow help\r\n");
		uart_puts("  ow scan\r\n");
		uart_puts("  ow readrom\r\n");
		uart_puts("  ow reset\r\n");
		return;
	}

	if (args[0] == 'h' && args[1] == 'e' && args[2] == 'l' && args[3] == 'p' && args[4] == '\0') {
		uart_puts("1-wire bus check:\r\n");
		uart_puts("  ow reset   : reset bus and report presence\r\n");
		uart_puts("  ow scan    : read ROM and show device family/serial\r\n");
		uart_puts("  ow readrom : read DS18B20 ROM bytes\r\n");
		return;
	}

	if (args[0] == 'r' && args[1] == 'e' && args[2] == 's' && args[3] == 'e' && args[4] == 't' && args[5] == '\0') {
		if (ow_reset()) {
			uart_puts("1W: device present\r\n");
		} else {
			uart_puts("1W: no device detected\r\n");
		}
		return;
	}

	if ((args[0] == 's' && args[1] == 'c' && args[2] == 'a' && args[3] == 'n' && args[4] == '\0') ||
		(args[0] == 'r' && args[1] == 'e' && args[2] == 'a' && args[3] == 'd' && args[4] == 'r' &&
		 args[5] == 'o' && args[6] == 'm' && args[7] == '\0')) {
		unsigned char rom[8];
		unsigned char i;
		if (!ow_reset()) {
			uart_puts("1W: no device detected\r\n");
			return;
		}
		ow_write_byte(0x33);
		for (i = 0; i < 8; i++) {
			rom[i] = ow_read_byte();
		}
		uart_puts("1W: ROM =");
		for (i = 0; i < 8; i++) {
			uart_putc(' ');
			ow_print_hex_byte(rom[i]);
		}
		uart_puts("\r\n");
		if (rom[0] == 0x28) {
			uart_puts("1W: DS18B20 family detected\r\n");
		} else {
			uart_puts("1W: unknown device family\r\n");
		}
		return;
	}

	uart_puts("ERR: unknown 1-wire command: ");
	uart_puts(args);
	uart_puts("\r\n");
}