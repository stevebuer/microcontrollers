# AX.25 Decoder for STM32F103C8T6 (Blue Pill)

An AX.25 packet radio decoder implementation using FreeRTOS on the STM32F103C8T6 microcontroller.

## Overview

This project implements a real-time AX.25 protocol decoder for Amateur Packet Radio (APRS) using FreeRTOS. The decoder can process Bell 202 AFSK (Audio Frequency Shift Keying) modulated signals at 1200 baud, commonly used in VHF packet radio.

## Hardware Requirements

- **STM32F103C8T6** "Blue Pill" development board
  - ARM Cortex-M3 @ 72 MHz
  - 64KB Flash, 20KB SRAM
  - USB, UART, I2C, SPI interfaces
  
- **Audio Input Circuit**
  - Low-pass filter for audio conditioning
  - ADC input (PA0 or similar)
  - Optional: Op-amp for signal conditioning

- **Optional Components**
  - OLED display (I2C) for packet display
  - SD card module for logging
  - USB-Serial for debugging

## Project Structure

```
ax25-decoder/
├── src/              # Source files
├── inc/              # Header files
├── docs/             # Documentation
├── FreeRTOS/         # FreeRTOS kernel (to be added)
├── STM32_HAL/        # STM32 HAL drivers (to be added)
└── Makefile          # Build configuration
```

## Features (Planned)

### Core Functionality
- [x] Project structure
- [ ] Bell 202 AFSK demodulation (1200 baud)
- [ ] HDLC frame synchronization and bit stuffing
- [ ] AX.25 packet decoding
- [ ] CRC validation
- [ ] Multi-level address parsing (up to 8 digipeaters)

### FreeRTOS Tasks
- **ADC Sampling Task**: Continuously sample audio input
- **Demodulator Task**: Process audio to extract digital bits
- **Frame Decoder Task**: Assemble bits into AX.25 frames
- **Display Task**: Output decoded packets
- **Logging Task**: Store packets to SD card (optional)

### Performance Targets
- Real-time processing at 1200 baud
- < 1% packet loss under good signal conditions
- Low latency (< 100ms from signal to decode)

## AX.25 Protocol Overview

AX.25 is the data link layer protocol for amateur packet radio:
- Based on X.25 protocol
- Uses HDLC framing
- Bell 202 modulation: 1200 Hz (mark/1), 2200 Hz (space/0)
- NRZI encoding
- Bit stuffing (insert 0 after five consecutive 1s)

## Build Instructions

### Prerequisites
```bash
# Install ARM toolchain
sudo apt-get install gcc-arm-none-eabi gdb-multiarch openocd

# Install FreeRTOS (instructions TBD)
# Install STM32CubeMX HAL (instructions TBD)
```

### Build
```bash
cd ax25-decoder
make
```

### Flash
```bash
make flash
```

## Development Roadmap

### Phase 1: Foundation (Current)
- [x] Project structure
- [ ] FreeRTOS integration
- [ ] Basic ADC sampling
- [ ] UART debug output

### Phase 2: Demodulation
- [ ] Bell 202 AFSK demodulator
- [ ] Tone detection (1200/2200 Hz)
- [ ] NRZI decoding
- [ ] Clock recovery

### Phase 3: Frame Processing
- [ ] HDLC frame sync
- [ ] Bit de-stuffing
- [ ] CRC-16 validation

### Phase 4: AX.25 Decoding
- [ ] Address field parsing
- [ ] Control field parsing
- [ ] PID field parsing
- [ ] Information field extraction

### Phase 5: Output & Enhancement
- [ ] Display interface
- [ ] Packet filtering
- [ ] Statistics collection
- [ ] Logging capability

## Technical Details

### Signal Processing Pipeline
```
Audio Input → ADC → Low-pass Filter → Tone Detection →
NRZI Decoder → Bit Stuffing Removal → HDLC Frame Sync →
CRC Check → AX.25 Parser → Output
```

### Memory Considerations
- Audio buffer: ~512 samples (circular buffer)
- Frame buffer: 330 bytes max (AX.25 max frame size)
- FreeRTOS heap: ~8KB recommended
- Stack per task: 256-512 bytes

## References

- [AX.25 Link Access Protocol v2.2](http://www.ax25.net/AX25.2.2-Jul%2098-2.pdf)
- [Bell 202 Modem Standard](https://en.wikipedia.org/wiki/Bell_202_modem)
- [FreeRTOS Documentation](https://www.freertos.org/Documentation/RTOS_book.html)
- [STM32F103C8T6 Reference Manual](https://www.st.com/resource/en/reference_manual/cd00171190.pdf)

## License

See LICENSE file for details.

## Author

Steve - Amateur Radio Enthusiast

## Contributing

This is a personal learning project, but suggestions and improvements are welcome!
