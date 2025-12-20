/**
 * @file ax25_decoder.c
 * @brief AX.25 Protocol Decoder Implementation
 * @author Steve
 * @date 2025
 */

#include "ax25_decoder.h"
#include <string.h>
#include <stdio.h>

/* CRC lookup table for faster calculation */
static const uint16_t crc_table[256] = {
    /* TODO: Generate CRC-CCITT lookup table */
};

/**
 * @brief Initialize AX.25 decoder
 */
void AX25_Init(void)
{
    /* Initialize any global state */
    /* TODO: Implementation */
}

/**
 * @brief Decode an AX.25 frame from raw bytes
 */
bool AX25_DecodeFrame(AX25Frame_t *frame)
{
    /* TODO: Implement frame decoding
     * 1. Parse destination address
     * 2. Parse source address  
     * 3. Parse digipeater addresses (if any)
     * 4. Extract control field
     * 5. Extract PID
     * 6. Extract information field
     * 7. Verify CRC
     */
    
    return false;  /* Placeholder */
}

/**
 * @brief Parse a single address field
 */
uint8_t AX25_ParseAddress(const uint8_t *data, AX25_Address_t *addr)
{
    /* Extract callsign (6 bytes, shifted right 1 bit) */
    for (int i = 0; i < 6; i++) {
        addr->callsign[i] = (data[i] >> 1) & 0x7F;
        /* Remove trailing spaces */
        if (addr->callsign[i] == ' ') {
            addr->callsign[i] = '\0';
        }
    }
    addr->callsign[6] = '\0';
    
    /* Extract SSID and control bits from byte 6 */
    addr->ssid = (data[6] >> 1) & 0x0F;
    addr->command = (data[6] >> 7) & 0x01;
    addr->response = (data[6] >> 5) & 0x01;
    addr->last = (data[6] & 0x01);
    
    return 7;  /* 7 bytes consumed */
}

/**
 * @brief Format address as string (e.g., "N0CALL-1")
 */
int AX25_FormatAddress(const AX25_Address_t *addr, char *buffer, size_t size)
{
    if (addr->ssid > 0) {
        return snprintf(buffer, size, "%s-%d", addr->callsign, addr->ssid);
    } else {
        return snprintf(buffer, size, "%s", addr->callsign);
    }
}

/**
 * @brief Format complete frame for display
 */
int AX25_FormatFrame(const AX25Frame_t *frame, char *buffer, size_t size)
{
    int len = 0;
    
    /* Source > Destination */
    len += AX25_FormatAddress(&frame->source, buffer + len, size - len);
    len += snprintf(buffer + len, size - len, " > ");
    len += AX25_FormatAddress(&frame->dest, buffer + len, size - len);
    
    /* Digipeater path */
    if (frame->num_digipeaters > 0) {
        len += snprintf(buffer + len, size - len, " via ");
        for (int i = 0; i < frame->num_digipeaters; i++) {
            if (i > 0) {
                len += snprintf(buffer + len, size - len, ",");
            }
            len += AX25_FormatAddress(&frame->digipeaters[i], 
                                     buffer + len, size - len);
        }
    }
    
    /* Control and PID */
    len += snprintf(buffer + len, size - len, 
                   " [Ctrl:0x%02X PID:0x%02X]", frame->control, frame->pid);
    
    /* Information field (if present) */
    if (frame->info_len > 0) {
        len += snprintf(buffer + len, size - len, "\nInfo: ");
        /* Print as ASCII (sanitized) */
        for (int i = 0; i < frame->info_len && len < size - 1; i++) {
            char c = frame->info[i];
            if (c >= 32 && c <= 126) {
                buffer[len++] = c;
            } else {
                buffer[len++] = '.';
            }
        }
    }
    
    /* CRC status */
    len += snprintf(buffer + len, size - len, "\nCRC: %s\n",
                   frame->fcs_valid ? "OK" : "FAIL");
    
    return len;
}

/**
 * @brief Calculate CRC-CCITT using lookup table
 */
uint16_t AX25_CalculateCRC(const uint8_t *data, uint16_t len)
{
    uint16_t crc = AX25_CRC_INIT;
    
    for (uint16_t i = 0; i < len; i++) {
        /* Using bit-by-bit calculation (slower but simpler) */
        /* TODO: Use lookup table for speed */
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ AX25_CRC_POLYNOMIAL;
            } else {
                crc = crc >> 1;
            }
        }
    }
    
    return crc ^ 0xFFFF;  /* Final XOR */
}

/**
 * @brief Verify frame CRC
 */
bool AX25_VerifyCRC(const uint8_t *data, uint16_t len, uint16_t received_crc)
{
    uint16_t calculated_crc = AX25_CalculateCRC(data, len);
    return (calculated_crc == received_crc);
}
