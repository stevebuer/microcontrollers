/*
 * NRZI Decoder for AX.25
 */

#include "nrzi.h"

void nrzi_init(nrzi_state_t *state)
{
    state->initialized = 0;

    state->prev_tone = TONE_MARK;  /* arbitrary; overwritten on first real bit */
}

uint8_t nrzi_decode_bit(nrzi_state_t *state, tone_t current_tone)
{
    uint8_t bit;

    if (!state->initialized) {

        state->prev_tone = current_tone;

        state->initialized = 1;

        return 1;   /* no transition possible yet — see header note */
    }

    bit = (current_tone == state->prev_tone) ? 1 : 0;

    state->prev_tone = current_tone;

    return bit;
}
