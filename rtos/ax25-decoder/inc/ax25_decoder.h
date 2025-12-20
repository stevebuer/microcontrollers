/**
 * @file ax25_decoder.h
 * @brief AX.25 Protocol Decoder Header
 * @author Steve
 * @date 2025
 */

#ifndef AX25_DECODER_H
#define AX25_DECODER_H

#include <stdint.h>
#include <stdbool.h>

/* AX.25 Constants */
#define AX25_FLAG               0x7E
#define AX25_MAX_FRAME_SIZE     330
#define AX25_MIN_FRAME_SIZE     18
#define AX25_ADDR_LEN           7
#define AX25_MAX_DIGIPEATERS    8
#define AX25_CRC_POLYNOMIAL     0x8408  /* Reversed 0x1021 */
#define AX25_CRC_INIT           0xFFFF

/* Frame types */
#define AX25_FRAME_TYPE_I       0x00    /* Information */
#define AX25_FRAME_TYPE_S       0x01    /* Supervisory */
#define AX25_FRAME_TYPE_U       0x03    /* Unnumbered */

/* Protocol IDs */
#define AX25_PID_NONE           0xF0
#define AX25_PID_X25            0x01
#define AX25_PID_COMPRESSED_TCP 0x06
#define AX25_PID_UNCOMPRESSED_TCP 0x07
#define AX25_PID_TEXNET         0xC3

/**
 * @brief AX.25 Address structure
 */
typedef struct {
    char callsign[7];       /* Callsign (6 chars + null) */
    uint8_t ssid;          /* SSID (0-15) */
    bool command;          /* Command bit */
    bool response;         /* Response bit */
    bool last;             /* Last address in chain */
} AX25_Address_t;

/**
 * @brief AX.25 Frame structure
 */
typedef struct {
    /* Address fields */
    AX25_Address_t dest;                    /* Destination address */
    AX25_Address_t source;                  /* Source address */
    AX25_Address_t digipeaters[AX25_MAX_DIGIPEATERS]; /* Digipeater path */
    uint8_t num_digipeaters;                /* Number of digipeaters */
    
    /* Control field */
    uint8_t control;                        /* Control byte */
    uint8_t frame_type;                     /* I, S, or U frame */
    
    /* Protocol ID */
    uint8_t pid;                            /* Protocol ID */
    
    /* Information field */
    uint8_t info[256];                      /* Information payload */
    uint16_t info_len;                      /* Length of information */
    
    /* Frame check sequence */
    uint16_t fcs_received;                  /* Received FCS */
    uint16_t fcs_calculated;                /* Calculated FCS */
    bool fcs_valid;                         /* FCS validation result */
    
    /* Metadata */
    uint32_t timestamp;                     /* Reception timestamp */
    int8_t rssi;                            /* Signal strength (if available) */
} AX25Frame_t;

/**
 * @brief Audio buffer structure
 */
typedef struct {
    int16_t samples[128];   /* Audio samples */
    uint16_t length;        /* Number of samples */
    uint32_t timestamp;     /* Buffer timestamp */
} AudioBuffer_t;

#define AUDIO_BUFFER_SIZE 128

/**
 * @brief Initialize AX.25 decoder
 */
void AX25_Init(void);

/**
 * @brief Decode an AX.25 frame
 * @param frame Pointer to frame structure to populate
 * @return true if frame is valid, false otherwise
 */
bool AX25_DecodeFrame(AX25Frame_t *frame);

/**
 * @brief Parse address field
 * @param data Raw address bytes
 * @param addr Pointer to address structure
 * @return Number of bytes consumed
 */
uint8_t AX25_ParseAddress(const uint8_t *data, AX25_Address_t *addr);

/**
 * @brief Format address as string
 * @param addr Address structure
 * @param buffer Output buffer
 * @param size Buffer size
 * @return Number of characters written
 */
int AX25_FormatAddress(const AX25_Address_t *addr, char *buffer, size_t size);

/**
 * @brief Format complete frame as human-readable string
 * @param frame Frame structure
 * @param buffer Output buffer
 * @param size Buffer size
 * @return Number of characters written
 */
int AX25_FormatFrame(const AX25Frame_t *frame, char *buffer, size_t size);

/**
 * @brief Calculate CRC-CCITT
 * @param data Data buffer
 * @param len Data length
 * @return Calculated CRC
 */
uint16_t AX25_CalculateCRC(const uint8_t *data, uint16_t len);

/**
 * @brief Verify frame CRC
 * @param data Frame data (excluding flags)
 * @param len Frame length
 * @param received_crc Received CRC value
 * @return true if CRC is valid
 */
bool AX25_VerifyCRC(const uint8_t *data, uint16_t len, uint16_t received_crc);

#endif /* AX25_DECODER_H */
