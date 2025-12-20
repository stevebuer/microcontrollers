# AX.25 Protocol Specification

## Overview

AX.25 is the data link layer protocol used in amateur packet radio. It is derived from the X.25 protocol and uses HDLC (High-Level Data Link Control) framing.

## Frame Structure

An AX.25 frame consists of the following fields:

```
┌──────┬─────────┬──────────┬──────────┬─────┬──────────────┬─────┬──────┐
│ Flag │ Address │ Address  │ Control  │ PID │ Information  │ FCS │ Flag │
│      │ Dest    │ Source   │          │     │              │     │      │
└──────┴─────────┴──────────┴──────────┴─────┴──────────────┴─────┴──────┘
  0x7E   7-70 bytes           1-2 bytes  1 byte  0-256 bytes  16-bit  0x7E
```

### Frame Fields

#### 1. Flag (0x7E)
- Binary: `01111110`
- Marks beginning and end of frame
- Single flag can serve as both end and start flag for adjacent frames

#### 2. Address Field (14-70 bytes)
Consists of:
- **Destination Address** (7 bytes)
- **Source Address** (7 bytes)
- **Digipeater Addresses** (0-8 × 7 bytes)

Each address subfield:
```
┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┬────────┐
│ Callsign (6 bytes, shifted left 1 bit)           │ SSID (1 byte)   │
└─────────┴─────────┴─────────┴─────────┴─────────┴─────────┴────────┘
  Byte 0    Byte 1    Byte 2    Byte 3    Byte 4    Byte 5    Byte 6

Byte 6 (SSID):
┌───┬───┬───┬───┬───┬───┬───┬───┐
│ C │ R │ R │SSID(4)│ 0 │EXT│
└───┴───┴───┴───────┴───┴───┘
```

- **Callsign**: ASCII characters, shifted left 1 bit, padded with spaces
- **SSID**: Secondary Station Identifier (0-15)
- **C**: Command/Response bit
- **RR**: Reserved bits
- **EXT**: Extension bit (0=more addresses follow, 1=last address)

#### 3. Control Field (1-2 bytes)
Defines frame type:

**Information Frame (I-frame):**
```
┌───┬───┬───┬───┬───┬───┬───┬───┐
│ 0 │ N(R) (3)  │ P │ N(S) (3)  │
└───┴───────────┴───┴───────────┘
```

**Supervisory Frame (S-frame):**
```
┌───┬───┬───┬───┬───┬───┬───┬───┐
│ 1 │ 0 │ S │ S │ P │ N(R) (3)  │
└───┴───┴───┴───┴───┴───────────┘
```

**Unnumbered Frame (U-frame):**
```
┌───┬───┬───┬───┬───┬───┬───┬───┐
│ 1 │ 1 │ M │ M │ P │ M │ M │ M │
└───┴───┴───┴───┴───┴───┴───┴───┘
```

- N(S): Send sequence number
- N(R): Receive sequence number
- P/F: Poll/Final bit
- S: Supervisory function bits
- M: Modifier function bits

#### 4. Protocol ID (PID) - 1 byte
Identifies the Layer 3 protocol:
- `0xF0`: No Layer 3 (most common in APRS)
- `0x01`: ISO 8208/CCITT X.25 PLP
- `0x06`: Compressed TCP/IP
- `0x07`: Uncompressed TCP/IP
- `0xC3`: TEXNET
- `0xCC`: APPLETALK
- `0xCD`: APPLETALK ARP

#### 5. Information Field (0-256 bytes)
- Contains the actual data payload
- Length varies based on frame type
- Optional (not present in some frame types)

#### 6. Frame Check Sequence (FCS) - 16 bits
- CRC-CCITT polynomial: x^16 + x^12 + x^5 + 1 (0x1021)
- Initial value: 0xFFFF
- Calculated over address, control, PID, and information fields
- Transmitted LSB first

## Physical Layer: Bell 202 AFSK

### Modulation
- **Mark (1)**: 1200 Hz
- **Space (0)**: 2200 Hz
- **Baud Rate**: 1200 baud
- **Bit Rate**: 1200 bps

### NRZI Encoding
- **No transition** = binary 1
- **Transition** = binary 0
- Used to maintain DC balance

### Example
```
Data bits:     1   0   1   1   0   1   0   0
NRZI:         ─┐ ┌─┐ ┌───┐ ┌─┐ ┌─┐ ┌─┐
              └─┘ └─┘     └─┘ └─┘ └─┘
```

## HDLC Bit Stuffing

To prevent flag sequences (01111110) from appearing in the data:
- **Rule**: After five consecutive 1s, insert a 0
- **Removal**: Receiver removes 0 after five consecutive 1s

### Example
```
Original:     0111111001111110
After stuff:  01111101001111101 0
                    ↑        ↑
               stuffed bits
```

## Decoding Algorithm

### Step-by-Step Process

1. **Acquire Signal**
   - Sample audio at 9600+ Hz
   - Apply low-pass filter (< 3 kHz)

2. **Tone Detection**
   - Detect 1200 Hz (mark) and 2200 Hz (space)
   - Methods: Goertzel algorithm, PLL, correlation

3. **NRZI Decoding**
   - Detect transitions
   - No transition = 1, Transition = 0

4. **Bit Synchronization**
   - Recover bit clock from data transitions
   - Adjust sampling phase

5. **Flag Detection**
   - Search for 0x7E (01111110) pattern
   - Marks start/end of frame

6. **Bit De-stuffing**
   - Remove stuffed 0s after five 1s

7. **Frame Assembly**
   - Collect bits into bytes
   - Continue until ending flag

8. **CRC Validation**
   - Calculate CRC over received frame
   - Compare with received FCS
   - Discard frame if mismatch

9. **Address Parsing**
   - Extract destination callsign/SSID
   - Extract source callsign/SSID
   - Extract digipeater path (if present)

10. **Control & PID Parsing**
    - Identify frame type
    - Extract protocol ID

11. **Information Extraction**
    - Extract payload data
    - Decode based on PID

## Common Frame Examples

### APRS Position Report (Unnumbered Information)
```
Flag:     0x7E
Dest:     "APRS  " (SSID=0)
Source:   "N0CALL" (SSID=1)
Digi1:    "WIDE1 " (SSID=1)
Digi2:    "WIDE2 " (SSID=1, EXT=1)
Control:  0x03 (UI frame)
PID:      0xF0 (No Layer 3)
Info:     "!4903.50N/07201.75W-Comment"
FCS:      [2 bytes]
Flag:     0x7E
```

## Timing Requirements

### Bit Timing (1200 baud)
- **Bit period**: 833.33 μs
- **Sample rate**: 9600 Hz (8 samples per bit)
- **Mark frequency**: 1200 Hz (period = 833.33 μs)
- **Space frequency**: 2200 Hz (period = 454.55 μs)

### Frame Timing
- **Minimum frame**: ~176 bits (22 bytes)
- **Maximum frame**: ~2640 bits (330 bytes)
- **Transmission time**: 146 ms - 2.2 seconds

## Implementation Notes

### Memory Requirements
- **Frame buffer**: 330 bytes (max frame size)
- **Audio buffer**: 512+ samples (circular buffer)
- **Bit buffer**: 64 bytes (for bit stuffing)

### Processing Requirements
- ADC sampling: 9600 Hz minimum
- Tone detection: Every 833 μs
- Bit decisions: 1200 times per second

### Error Handling
- CRC failures: Discard frame silently
- Invalid addresses: Log and discard
- Buffer overflows: Reset decoder state

## References

- AX.25 Link Access Protocol for Amateur Packet Radio v2.2
- HDLC ISO/IEC 13239
- Bell 202 Modem Standard
- APRS Protocol Reference v1.0.1

## Tools for Testing

- **Direwolf**: Software TNC for generating test signals
- **Xastir**: APRS client
- **YAAC**: Yet Another APRS Client
- **Audacity**: For audio analysis and test signal generation
