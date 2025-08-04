/*
 * micromorse
 *
 * keyer for linux console / pc speaker
 *
 * Steve Buer, N7MKO
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "micromorse.h"

#define MICROS_PER_MILLI 1000

void alsa_begin()
{
	/* todo */
}

void alsa_end()
{
	/* close */
}

void dah()
{
        printf("-");
	// alsa_play() sine wave DOT_TIME
	usleep(DOT_TIME*MICROS_PER_MILLI);
}

void dit()
{
        printf(".");
	// alsa_play() sine wave DASH_TIME
	usleep(DOT_TIME*MICROS_PER_MILLI);
}

void word_space()
{
	usleep(300*1000); 
}
