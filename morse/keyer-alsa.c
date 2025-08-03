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

/* todo: calculate wpm and timing */

/* todo: open audio dev and play a sine wave */

void dah()
{
        printf("dah ");
	system("play -n synth 0.6 sine 750 vol 0.5");
	// usleep(200*1000); // 200 msec
}

void dit()
{
        printf("dit ");
	system("play -n synth 0.2 sine 750 vol 0.5");
	// usleep(200*1000); // 200 msec
}

void space()
{
	usleep(300*1000); // 200 msec
}
