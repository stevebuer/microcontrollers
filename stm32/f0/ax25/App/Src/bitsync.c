#include "bitsync.h"

void bitsync_init(bitsync_state_t *state)
{
    state->initialized = 0;

    state->phase = 0;
}

uint8_t bitsync_step(bitsync_state_t *state, tone_t sample_tone, tone_t *bit_tone_out)
{
    if (!state->initialized) {

        state->last_sample_tone = sample_tone;
        state->initialized = 1;
        state->phase = 0;
        return 0;
    }

    if (sample_tone != state->last_sample_tone) {

        state->phase = 0;                 /* re-sync on transition edge */
        state->last_sample_tone = sample_tone;

    } else {

        state->phase++;

        if (state->phase >= SAMPLES_PER_BIT) {
            state->phase = 0;             /* no transition; assume period restarts */
        }
    }

    if (state->phase == BITSYNC_SAMPLE_PT) {

        *bit_tone_out = sample_tone;
        return 1;
    }

    return 0;
}
