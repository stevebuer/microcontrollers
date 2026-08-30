#ifndef GOERTZEL_H
#define GOERTZEL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Tuned for 9600 Hz sample rate, 1200 baud AX.25 (8 samples/bit). */
#define GOERTZEL_WIN    8
#define GOERTZEL_SHIFT  14   /* fixed-point scale: coeffs are Q(GOERTZEL_SHIFT) */

/* coeff = round(2*cos(2*pi*f/fs) * 2^GOERTZEL_SHIFT)
 * MARK  (1200 Hz @ 9600 Hz): 2*cos(45.0 deg)  = 1.41421 -> 23170
 * SPACE (2200 Hz @ 9600 Hz): 2*cos(82.5 deg)  = 0.26105 ->  4278
 * Recompute both if you change sample rate or tone frequencies. */
#define GOERTZEL_COEFF_MARK   23170
#define GOERTZEL_COEFF_SPACE   4278

typedef enum {
    TONE_MARK = 0,
    TONE_SPACE = 1
} tone_t;

/* Raw power (proportional to squared magnitude) at one target frequency,
 * over GOERTZEL_WIN samples starting at raw[start]. dc_bias is subtracted
 * from every sample before processing (see goertzel_estimate_bias). */
int32_t goertzel_power_raw(const uint16_t *raw, uint16_t start,
                            int16_t coeff, int16_t dc_bias);

/* Computes both tone powers over one window and returns which tone won.
 * total_power_out (optional, pass NULL to skip) gives mark+space power,
 * useful as a DCD/carrier-detect metric. */
tone_t goertzel_classify(const uint16_t *raw, uint16_t start,
                          int16_t dc_bias, int32_t *total_power_out);

/* Slides a GOERTZEL_WIN window across an entire block, one sample at a
 * time, filling tone_out[] and (optionally) power_out[] with one result
 * per starting position. Both arrays must be at least
 * (block_len - GOERTZEL_WIN + 1) entries long. */
void goertzel_process_block(const uint16_t *raw, uint16_t block_len,
                             int16_t dc_bias, tone_t *tone_out,
                             int32_t *power_out);

/* Averages len raw ADC samples to estimate the DC bias point (should land
 * near 2048 on a 12-bit ADC with your VDD/2 bias network, but measuring it
 * rather than hardcoding it accounts for R1/R2 tolerance and ADC offset). */
int16_t goertzel_estimate_bias(const uint16_t *raw, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif /* GOERTZEL_H */
