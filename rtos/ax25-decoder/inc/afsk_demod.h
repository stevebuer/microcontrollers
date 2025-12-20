/**
 * @file afsk_demod.h
 * @brief AFSK (Bell 202) Demodulator Header
 * @author Steve
 * @date 2025
 * 
 * Demodulates Bell 202 AFSK signals (1200 baud)
 * Mark = 1200 Hz, Space = 2200 Hz
 */

#ifndef AFSK_DEMOD_H
#define AFSK_DEMOD_H

#include <stdint.h>
#include <stdbool.h>

/* AFSK Parameters */
#define AFSK_SAMPLE_RATE        9600    /* Hz */
#define AFSK_BAUD_RATE          1200    /* baud */
#define AFSK_MARK_FREQ          1200    /* Hz */
#define AFSK_SPACE_FREQ         2200    /* Hz */

#define AFSK_SAMPLES_PER_BIT    (AFSK_SAMPLE_RATE / AFSK_BAUD_RATE)

/* Demodulator state */
typedef enum {
    DEMOD_STATE_IDLE,
    DEMOD_STATE_SYNCING,
    DEMOD_STATE_LOCKED
} DemodState_t;

/**
 * @brief AFSK Demodulator context
 */
typedef struct {
    /* Tone detectors */
    int32_t mark_mag;       /* Mark tone magnitude */
    int32_t space_mag;      /* Space tone magnitude */
    
    /* Bit clock recovery */
    uint32_t bit_clock;     /* Bit timing counter */
    uint32_t sample_count;  /* Sample counter */
    
    /* NRZI decoder */
    uint8_t last_bit;       /* Last bit value for NRZI */
    
    /* State */
    DemodState_t state;     /* Demodulator state */
    
    /* Statistics */
    uint32_t bits_decoded;  /* Total bits decoded */
    uint32_t sync_errors;   /* Synchronization errors */
} AFSK_Context_t;

/**
 * @brief Initialize AFSK demodulator
 */
void AFSK_Init(void);

/**
 * @brief Process a single audio sample
 * @param sample Audio sample value (12-bit ADC)
 * @param bit_out Pointer to output bit (if ready)
 * @return true if bit is ready, false otherwise
 */
bool AFSK_ProcessSample(int16_t sample, uint8_t *bit_out);

/**
 * @brief Detect tone frequency using Goertzel algorithm
 * @param sample Audio sample
 * @param freq Target frequency
 * @return Tone magnitude
 */
int32_t AFSK_GoertzelDetect(int16_t sample, uint16_t freq);

/**
 * @brief Decode NRZI to NRZ
 * @param nrzi_bit NRZI encoded bit
 * @return NRZ decoded bit
 */
uint8_t AFSK_DecodeNRZI(uint8_t nrzi_bit);

/**
 * @brief Reset demodulator state
 */
void AFSK_Reset(void);

/**
 * @brief Get demodulator statistics
 * @param ctx Pointer to context to fill
 */
void AFSK_GetStats(AFSK_Context_t *ctx);

#endif /* AFSK_DEMOD_H */
