/**
 * @file afsk_demod.c
 * @brief AFSK Demodulator Implementation
 * @author Steve
 * @date 2025
 */

#include "afsk_demod.h"
#include <string.h>

/* Global demodulator context */
static AFSK_Context_t g_afsk_ctx;

/**
 * @brief Initialize AFSK demodulator
 */
void AFSK_Init(void)
{
    memset(&g_afsk_ctx, 0, sizeof(AFSK_Context_t));
    g_afsk_ctx.state = DEMOD_STATE_IDLE;
}

/**
 * @brief Process a single audio sample
 */
bool AFSK_ProcessSample(int16_t sample, uint8_t *bit_out)
{
    bool bit_ready = false;
    
    /* Detect mark and space tones */
    g_afsk_ctx.mark_mag = AFSK_GoertzelDetect(sample, AFSK_MARK_FREQ);
    g_afsk_ctx.space_mag = AFSK_GoertzelDetect(sample, AFSK_SPACE_FREQ);
    
    /* Increment sample counter */
    g_afsk_ctx.sample_count++;
    
    /* Check if it's time to make a bit decision */
    if (g_afsk_ctx.sample_count >= AFSK_SAMPLES_PER_BIT) {
        /* Make bit decision based on tone magnitudes */
        uint8_t nrzi_bit = (g_afsk_ctx.mark_mag > g_afsk_ctx.space_mag) ? 1 : 0;
        
        /* Decode NRZI to get actual bit */
        *bit_out = AFSK_DecodeNRZI(nrzi_bit);
        
        /* Reset counter for next bit */
        g_afsk_ctx.sample_count = 0;
        g_afsk_ctx.bits_decoded++;
        
        bit_ready = true;
    }
    
    return bit_ready;
}

/**
 * @brief Goertzel algorithm for tone detection
 * Simplified single-sample version (needs proper windowing for production)
 */
int32_t AFSK_GoertzelDetect(int16_t sample, uint16_t freq)
{
    /* TODO: Implement proper Goertzel algorithm
     * This is a placeholder - real implementation needs:
     * - Windowing (e.g., Hamming window)
     * - Proper state variables (Q1, Q2)
     * - Coefficient calculation based on frequency
     * - Magnitude calculation
     */
    
    /* Placeholder: just return sample value */
    return (int32_t)sample;
}

/**
 * @brief Decode NRZI to NRZ
 */
uint8_t AFSK_DecodeNRZI(uint8_t nrzi_bit)
{
    uint8_t nrz_bit;
    
    /* NRZI decoding:
     * If current bit == last bit: output 1 (no transition)
     * If current bit != last bit: output 0 (transition)
     */
    if (nrzi_bit == g_afsk_ctx.last_bit) {
        nrz_bit = 1;
    } else {
        nrz_bit = 0;
    }
    
    /* Update last bit */
    g_afsk_ctx.last_bit = nrzi_bit;
    
    return nrz_bit;
}

/**
 * @brief Reset demodulator state
 */
void AFSK_Reset(void)
{
    g_afsk_ctx.state = DEMOD_STATE_IDLE;
    g_afsk_ctx.sample_count = 0;
    g_afsk_ctx.bit_clock = 0;
    g_afsk_ctx.last_bit = 0;
}

/**
 * @brief Get demodulator statistics
 */
void AFSK_GetStats(AFSK_Context_t *ctx)
{
    if (ctx != NULL) {
        memcpy(ctx, &g_afsk_ctx, sizeof(AFSK_Context_t));
    }
}
