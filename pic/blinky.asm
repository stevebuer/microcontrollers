;
; PIC16 Blinky
;

LIST p=PIC16f690

ERRORLEVEL -302

#include <p16f690.inc>

	goto main

main:
	bsf STATUS, RP0

	movlw 0
	movfw TRISB

; to do -- loop
	
	end
