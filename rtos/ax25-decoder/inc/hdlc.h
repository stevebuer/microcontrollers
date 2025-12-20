/**
 * @file hdlc.h
 * @brief HDLC Frame Processing Header
 * @author Steve
 * @date 2025
 * 
 * Handles HDLC flag detection, bit stuffing, and frame assembly
 */

#ifndef HDLC_H
#define HDLC_H

#include <stdint.h>
#include <stdbool.h>
#include "ax25_decoder.h"

/* HDLC Constants */
#define HDLC_FLAG           0x7E    /* 01111110 */
#define HDLC_MAX_ONES       5       /* Maximum consecutive 1s before stuffing */

/* HDLC State Machine */
typedef enum {
    HDLC_STATE_HUNT,        /* Looking for flag */
    HDLC_STATE_SYNC,        /* Flag found, waiting for data */
    HDLC_STATE_DATA,        /* Receiving frame data */
    HDLC_STATE_FCS          /* Receiving frame check sequence */
} HDLC_State_t;

/**
 * @brief HDLC Decoder context
 */
typedef struct {
    /* State machine */
    HDLC_State_t state;         /* Current state */
    
    /* Bit assembly */
    uint8_t shift_reg;          /* Bit shift register */
    uint8_t bit_count;          /* Bits in shift register */
    uint8_t ones_count;         /* Consecutive 1s counter */
    
    /* Frame buffer */
    uint8_t frame_buffer[AX25_MAX_FRAME_SIZE];
    uint16_t frame_index;       /* Current position in buffer */
    uint16_t frame_length;      /* Total frame length */
    
    /* Statistics */
    uint32_t frames_received;   /* Total frames received */
    uint32_t frames_valid;      /* Valid frames */
    uint32_t frames_invalid;    /* Invalid frames (CRC error) */
    uint32_t aborts;            /* Frame aborts */
} HDLC_Context_t;

/**
 * @brief Initialize HDLC decoder
 */
void HDLC_Init(void);

/**
 * @brief Process a single bit
 * @param bit Input bit
 * @param frame Pointer to frame structure (filled when complete)
 * @return true if complete frame is ready, false otherwise
 */
bool HDLC_ProcessBit(uint8_t bit, AX25Frame_t *frame);

/**
 * @brief Detect HDLC flag pattern
 * @param shift_reg 8-bit shift register
 * @return true if flag detected
 */
bool HDLC_DetectFlag(uint8_t shift_reg);

/**
 * @brief Handle bit stuffing
 * @param bit Input bit
 * @return true if bit should be kept, false if it's a stuffed bit
 */
bool HDLC_HandleBitStuffing(uint8_t bit);

/**
 * @brief Reset HDLC decoder state
 */
void HDLC_Reset(void);

/**
 * @brief Get HDLC statistics
 * @param ctx Pointer to context to fill
 */
void HDLC_GetStats(HDLC_Context_t *ctx);

/**
 * @brief Dump frame buffer (for debugging)
 * @param buffer Output buffer
 * @param size Buffer size
 * @return Number of characters written
 */
int HDLC_DumpFrame(char *buffer, size_t size);

#endif /* HDLC_H */
