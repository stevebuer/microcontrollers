;
; 8051 blink with timer
;
; AS31 assembler
;

; Interrupt vectors

.ORG 0000H
	LJMP BEGIN
.ORG 000BH
	LJMP BLINK

; Main

.ORG 40

BEGIN:  MOV P1, #0
        SETB P1.2
        MOV TMOD, #00000001B
	SETB TR0

; Timer

BLINK:  CPL P1.2
	RET
