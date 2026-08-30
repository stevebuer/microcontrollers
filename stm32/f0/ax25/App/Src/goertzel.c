#include "goertzel.h"

int32_t goertzel_power_raw(const uint16_t *raw, uint16_t start,
                            int16_t coeff, int16_t dc_bias)
{
    int32_t q0, q1 = 0, q2 = 0;

    for (uint16_t i = 0; i < GOERTZEL_WIN; i++) {
        int32_t sample = (int32_t)raw[start + i] - dc_bias;
        q0 = (((int32_t)coeff * q1) >> GOERTZEL_SHIFT) - q2 + sample;
        q2 = q1;
        q1 = q0;
    }

    int32_t coeff_q1 = ((int32_t)coeff * q1) >> GOERTZEL_SHIFT;
    return (q1 * q1) + (q2 * q2) - (coeff_q1 * q2);
}

tone_t goertzel_classify(const uint16_t *raw, uint16_t start,
                          int16_t dc_bias, int32_t *total_power_out)
{
    int32_t p_mark  = goertzel_power_raw(raw, start, GOERTZEL_COEFF_MARK,  dc_bias);
    int32_t p_space = goertzel_power_raw(raw, start, GOERTZEL_COEFF_SPACE, dc_bias);

    if (total_power_out) {
        *total_power_out = p_mark + p_space;
    }
    return (p_space > p_mark) ? TONE_SPACE : TONE_MARK;
}

void goertzel_process_block(const uint16_t *raw, uint16_t block_len,
                             int16_t dc_bias, tone_t *tone_out,
                             int32_t *power_out)
{
    uint16_t last_start = block_len - GOERTZEL_WIN;

    for (uint16_t n = 0; n <= last_start; n++) {
        int32_t total;
        tone_out[n] = goertzel_classify(raw, n, dc_bias, &total);
        if (power_out) {
            power_out[n] = total;
        }
    }
}

int16_t goertzel_estimate_bias(const uint16_t *raw, uint16_t len)
{
    uint32_t sum = 0;
    for (uint16_t i = 0; i < len; i++) {
        sum += raw[i];
    }
    return (int16_t)(sum / len);
}
