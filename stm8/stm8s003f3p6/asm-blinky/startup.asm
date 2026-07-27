	.module startup

	.area HOME (ABS)
	.org 0x8000          ; Flash start for STM8S003

__reset:
	ldw   x, #0x03FF
	ldw   sp, x
	jp    _main          ; jump to main()

;    .area VECTORS (ABS)
;    .org 0x8000 + 0x8000 ; interrupt vector table location depends on memory map
