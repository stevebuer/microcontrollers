;
; 8051 serial port test pattern for scope debugging
;
; AS31 assembler
;

BEGIN:
	MOV SCON, #50H          ; UART mode 1, REN=1
	MOV TMOD, #20H          ; Timer1, mode2
	MOV TH1, #0DFH          ; reload for 9600 baud @ 12 MHz
	MOV TL1, #0DFH
	ANL PCON, #7FH          ; SMOD = 0
	SETB TR1
HERE:
        MOV SBUF, #'U'          ; 0x55 = 01010101b
WAIT:
        JNB TI, WAIT
        CLR TI
        SJMP HERE
