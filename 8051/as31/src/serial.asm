;
; 8051 uart control
;

.EQU LINE, 80H
.EQU SIZE, 16

.ORG 0H
	LJMP BEGIN

.ORG 30H

BEGIN:	
	MOV SCON, #50H		; mode 1, REN = 1
	MOV TMOD, #20H		; timer mode 2
	MOV TH1, #0FDH		; 9600 baud @ 11.0592 MHz
	SETB TR1		; start timer 1

	MOV R0, #LINE		; set read pointer

PROMPT: 
	MOV SBUF, #'>'
WAIT:   
	JNB TI, WAIT
	CLR TI

READ:	
	JNB RI, READ
	MOV A, SBUF
	CLR RI
	MOV @R0, A
	INC R0
	CJNE R0, #LINE+SIZE, NEXT
WRAP:	
	MOV R0, #LINE		; wrap buffer
NEXT:	
	SJMP READ

; to do: check end of line char
; set line ready bit var
