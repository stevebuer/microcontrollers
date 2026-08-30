#ifndef NRZI_H
#define NRZI_H

#include <stdint.h>
#include "goertzel.h"   /* for tone_t */

#ifdef __cplusplus
extern "C" {
#endif

/* NRZI decode state — just remembers the last bit-period's tone so the
 * next call can detect transition vs. no-transition. */
typedef struct {
    tone_t  prev_tone;
    uint8_t initialized;   /* 0 until the first real tone has been seen */
} nrzi_state_t;

/* Resets/starts a decode session. Call once at the start of each new
 * frame acquisition (e.g. right after DCD goes active / flag sync found),
 * since NRZI decoding is only meaningful relative to a known starting
 * reference tone. */
void nrzi_init(nrzi_state_t *state);

/* Feeds one recovered bit-period's tone in, returns the decoded logical
 * bit per AX.25 NRZI convention: same tone as previous period -> 1,
 * tone changed -> 0.
 *
 * The very first call after nrzi_init() has no previous tone to compare
 * against; it establishes the reference and returns 1 by convention.
 * This is harmless downstream — HDLC flag/CRC framing doesn't depend on
 * that first bit being "correct" in isolation, only on the overall
 * stream lining up once real flag bytes start arriving. */
uint8_t nrzi_decode_bit(nrzi_state_t *state, tone_t current_tone);

#ifdef __cplusplus
}
#endif

#endif /* NRZI_H */
