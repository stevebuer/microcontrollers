;
; PIC12F675 Blink
; Steve Buer
;

include <p12f675.inc>

ERRORLEVEL 0, -302

; LED on Pin 5 (GPIO2)

LED EQU B'000100'

; Setup GPIO Bank 0

	CLRF GPIO

; Comparator control CM2:CM0 --> off

	MOVLW B'111'
	MOVWF CMCON

; Switch to Bank 1

	BSF STATUS, RP0

; Select digital I/O not A/D converter input for AN0:AN3
 
	CLRF ANSEL

; All pins output

	MOVLW 0
	MOVWF TRISIO

	END	

