/* cortex m0 crt0 */

#include <stdint.h>

extern void main();

void _start(void) 
{
	/* linker defined symbols */

	extern uint32_t _sidata;  /* start of .data in Flash */
	extern uint32_t _sdata;   /* start of .data in RAM */
	extern uint32_t _edata;   /* end of .data in RAM */
	extern uint32_t _sbss;    /* start of .bss */
	extern uint32_t _ebss;    /* end of .bss */

	/* copy .data from Flash to RAM */

	uint32_t *src = &_sidata;
	uint32_t *dst = &_sdata;

	while (dst < &_edata)
        	*dst++ = *src++;

	/* zero .bss */

	uint32_t *b = &_sbss;

	while (b < &_ebss)
		*b++ = 0;

	system_init();

	main();

	while (1);
}
