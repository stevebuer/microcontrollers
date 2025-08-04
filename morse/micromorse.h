/*
 * micromorse: morse code routines for microcontrollers
 *
 * Steve Buer, N7MKO
 */

#define WPM 20

#define DOT_TIME 1.2/WPM*1000	/* dot time in milliseconds */
#define DOT_SPACE DOT_TIME
#define DASH_TIME 3*DOT_TIME
#define LETTER_SPACE 3*DOT_TIME
#define WORD_SPACE (7-1)*DOT_TIME	/* 1 dot pause built in to dit/dah functions */

/* microcontroller-specific keying implementations */

extern void dit();
extern void dah();
extern void word_space();
