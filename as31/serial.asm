;
; 8051 serial port write
;
; AS31 assembler
;

.EQU CMD, 080H

; Interrupt vectors

.ORG 0000H
	LJMP BEGIN

; Main

.ORG 40

BEGIN:
	MOV R0, CMD
        MOV TMOD, #00100000B
	MOV TH1, #11111101B
	MOV SCON, #1010000B
	SETB TR1

PROMPT: MOV SBUF, #'>'
	CLR TI 
WAIT:   JNB TI, WAIT

READ:	JNB RI, READ
	MOV A, SBUF
	MOV @R0, A
	AJMP PROMPT
