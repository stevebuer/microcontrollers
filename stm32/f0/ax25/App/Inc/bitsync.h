#ifndef BITSYNC_H
#define BITSYNC_H

#include <stdint.h>
#include "goertzel.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SAMPLES_PER_BIT   8   /* matches 9600 Hz / 1200 baud */
#define BITSYNC_SAMPLE_PT 4   /* sample mid-bit, away from edges */

typedef struct {
    tone_t  last_sample_tone;
    uint8_t phase;
    uint8_t initialized;
} bitsync_state_t;

void bitsync_init(bitsync_state_t *state);

/* Feed one raw per-sample tone in. Returns 1 and writes *bit_tone_out
 * when a bit-period decision is ready; returns 0 otherwise. Naive v1:
 * resets phase on every detected transition rather than a true DPLL —
 * fine for clean signals, first candidate to upgrade if real captures
 * show missed sync on marginal signal. */
uint8_t bitsync_step(bitsync_state_t *state, tone_t sample_tone,
                      tone_t *bit_tone_out);

#ifdef __cplusplus
}
#endif

#endif /* BITSYNC_H */
