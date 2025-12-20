/**
 * @file hdlc.c
 * @brief HDLC Frame Processing Implementation
 * @author Steve
 * @date 2025
 */

#include "hdlc.h"
#include <string.h>

/* Global HDLC context */
static HDLC_Context_t g_hdlc_ctx;

/**
 * @brief Initialize HDLC decoder
 */
void HDLC_Init(void)
{
    memset(&g_hdlc_ctx, 0, sizeof(HDLC_Context_t));
    g_hdlc_ctx.state = HDLC_STATE_HUNT;
}

/**
 * @brief Process a single bit through HDLC decoder
 */
bool HDLC_ProcessBit(uint8_t bit, AX25Frame_t *frame)
{
    bool frame_complete = false;
    
    /* Shift bit into register */
    g_hdlc_ctx.shift_reg = (g_hdlc_ctx.shift_reg >> 1) | (bit << 7);
    g_hdlc_ctx.bit_count++;
    
    /* Track consecutive 1s for bit stuffing */
    if (bit == 1) {
        g_hdlc_ctx.ones_count++;
    } else {
        g_hdlc_ctx.ones_count = 0;
    }
    
    /* Check for flag */
    if (HDLC_DetectFlag(g_hdlc_ctx.shift_reg)) {
        if (g_hdlc_ctx.state == HDLC_STATE_DATA) {
            /* End of frame */
            if (g_hdlc_ctx.frame_index >= AX25_MIN_FRAME_SIZE) {
                /* Copy frame data to output */
                /* TODO: Parse frame into AX25Frame_t structure */
                g_hdlc_ctx.frames_received++;
                frame_complete = true;
            }
        }
        
        /* Reset for next frame */
        g_hdlc_ctx.state = HDLC_STATE_SYNC;
        g_hdlc_ctx.frame_index = 0;
        g_hdlc_ctx.bit_count = 0;
        g_hdlc_ctx.ones_count = 0;
        
        return frame_complete;
    }
    
    /* Handle bit stuffing */
    if (g_hdlc_ctx.ones_count == 6) {
        /* Abort sequence (6 consecutive 1s without a 0) */
        g_hdlc_ctx.state = HDLC_STATE_HUNT;
        g_hdlc_ctx.aborts++;
        g_hdlc_ctx.ones_count = 0;
        return false;
    }
    
    if (g_hdlc_ctx.ones_count == 5 && bit == 0) {
        /* This is a stuffed bit - discard it */
        g_hdlc_ctx.ones_count = 0;
        return false;
    }
    
    /* Accumulate byte */
    if (g_hdlc_ctx.state == HDLC_STATE_DATA && g_hdlc_ctx.bit_count == 8) {
        /* Store byte in frame buffer */
        if (g_hdlc_ctx.frame_index < AX25_MAX_FRAME_SIZE) {
            g_hdlc_ctx.frame_buffer[g_hdlc_ctx.frame_index++] = 
                g_hdlc_ctx.shift_reg;
        } else {
            /* Buffer overflow - abort */
            g_hdlc_ctx.state = HDLC_STATE_HUNT;
            g_hdlc_ctx.aborts++;
        }
        g_hdlc_ctx.bit_count = 0;
    } else if (g_hdlc_ctx.state == HDLC_STATE_SYNC && g_hdlc_ctx.bit_count >= 8) {
        /* First byte after flag - start receiving data */
        g_hdlc_ctx.state = HDLC_STATE_DATA;
        g_hdlc_ctx.frame_buffer[g_hdlc_ctx.frame_index++] = 
            g_hdlc_ctx.shift_reg;
        g_hdlc_ctx.bit_count = 0;
    }
    
    return frame_complete;
}

/**
 * @brief Detect HDLC flag (0x7E)
 */
bool HDLC_DetectFlag(uint8_t shift_reg)
{
    return (shift_reg == HDLC_FLAG);
}

/**
 * @brief Reset HDLC decoder
 */
void HDLC_Reset(void)
{
    g_hdlc_ctx.state = HDLC_STATE_HUNT;
    g_hdlc_ctx.frame_index = 0;
    g_hdlc_ctx.bit_count = 0;
    g_hdlc_ctx.ones_count = 0;
}

/**
 * @brief Get HDLC statistics
 */
void HDLC_GetStats(HDLC_Context_t *ctx)
{
    if (ctx != NULL) {
        memcpy(ctx, &g_hdlc_ctx, sizeof(HDLC_Context_t));
    }
}

/**
 * @brief Dump frame buffer for debugging
 */
int HDLC_DumpFrame(char *buffer, size_t size)
{
    int len = 0;
    
    len += snprintf(buffer + len, size - len, 
                   "Frame length: %d bytes\n", g_hdlc_ctx.frame_index);
    
    /* Dump hex */
    for (int i = 0; i < g_hdlc_ctx.frame_index && len < size - 4; i++) {
        len += snprintf(buffer + len, size - len, 
                       "%02X ", g_hdlc_ctx.frame_buffer[i]);
        if ((i + 1) % 16 == 0) {
            buffer[len++] = '\n';
        }
    }
    
    return len;
}
