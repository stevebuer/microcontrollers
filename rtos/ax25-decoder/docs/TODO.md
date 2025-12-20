# Project TODO

## Immediate Next Steps

### 1. Development Environment Setup
- [ ] Install ARM toolchain (`gcc-arm-none-eabi`)
- [ ] Install OpenOCD for programming
- [ ] Install ST-Link utilities
- [ ] Set up serial terminal (minicom, screen, etc.)

### 2. FreeRTOS Integration
- [ ] Download FreeRTOS kernel
- [ ] Create `FreeRTOSConfig.h`
- [ ] Add FreeRTOS sources to Makefile
- [ ] Verify basic task scheduling

### 3. STM32 HAL Integration
- [ ] Download STM32CubeF1
- [ ] Extract HAL drivers
- [ ] Create linker script (`STM32F103C8Tx_FLASH.ld`)
- [ ] Create startup file (`startup_stm32f103xb.s`)
- [ ] Implement `SystemClock_Config()`

### 4. Peripheral Initialization
- [ ] Implement GPIO initialization (LED on PC13)
- [ ] Implement ADC initialization (PA0)
- [ ] Implement UART initialization (115200 baud)
- [ ] Test basic I/O

## Phase 1: Basic Functionality

### Audio Capture
- [ ] Configure ADC for continuous conversion
- [ ] Set up Timer2 for ADC trigger (9600 Hz)
- [ ] Implement DMA for ADC data transfer
- [ ] Test audio sampling with simple tone

### AFSK Demodulator
- [ ] Implement Goertzel algorithm for 1200/2200 Hz
- [ ] Add windowing function (Hamming)
- [ ] Implement bit clock recovery
- [ ] Test with generated AFSK tones

### NRZI Decoder
- [ ] Complete NRZI decoding logic
- [ ] Add transition detection
- [ ] Test with known bit patterns

## Phase 2: Frame Processing

### HDLC Decoder
- [ ] Complete flag detection
- [ ] Implement bit de-stuffing
- [ ] Add frame buffer management
- [ ] Handle abort sequences
- [ ] Test with known HDLC frames

### CRC Validation
- [ ] Generate CRC lookup table
- [ ] Implement CRC-CCITT calculation
- [ ] Add frame validation
- [ ] Test with valid/invalid frames

## Phase 3: AX.25 Decoding

### Address Parsing
- [ ] Implement address field parser
- [ ] Handle multiple digipeaters
- [ ] Extract callsign and SSID
- [ ] Parse control bits

### Frame Decoding
- [ ] Parse control field
- [ ] Extract PID
- [ ] Extract information field
- [ ] Format output for display

## Phase 4: Output and Display

### UART Output
- [ ] Format frames as text
- [ ] Add timestamp
- [ ] Implement statistics display
- [ ] Add debug output options

### OLED Display (Optional)
- [ ] Integrate u8g2 library
- [ ] Design display layout
- [ ] Show decoded packets
- [ ] Show signal quality

## Phase 5: Optimization and Testing

### Performance Tuning
- [ ] Optimize Goertzel implementation
- [ ] Tune filter parameters
- [ ] Adjust task priorities
- [ ] Reduce CPU usage

### Testing
- [ ] Test with Direwolf-generated signals
- [ ] Test with real radio signals
- [ ] Measure packet error rate
- [ ] Stress test with high traffic

### Documentation
- [ ] Document signal levels
- [ ] Create setup guide
- [ ] Write troubleshooting guide
- [ ] Add usage examples

## Phase 6: Advanced Features

### Signal Quality
- [ ] Add AGC (Automatic Gain Control)
- [ ] Implement noise blanker
- [ ] Add signal strength indicator
- [ ] Measure BER (Bit Error Rate)

### Logging
- [ ] Add SD card support
- [ ] Implement packet logging
- [ ] Add statistics logging
- [ ] Create log rotation

### Configuration
- [ ] Add configuration menu
- [ ] Store settings in EEPROM/Flash
- [ ] Add frequency offset adjustment
- [ ] Tunable filter parameters

## Known Issues / TODO in Code

### main.c
- [ ] Complete `SystemClock_Config()` implementation
- [ ] Complete `GPIO_Init()` implementation
- [ ] Complete `ADC_Init()` implementation
- [ ] Complete `UART_Init()` implementation
- [ ] Add error handling in task creation

### afsk_demod.c
- [ ] Implement proper Goertzel algorithm
- [ ] Add windowing function
- [ ] Implement clock recovery
- [ ] Add sync detection

### hdlc.c
- [ ] Complete frame assembly logic
- [ ] Add proper buffer overflow handling
- [ ] Implement frame validation
- [ ] Add statistics collection

### ax25_decoder.c
- [ ] Complete frame decoding logic
- [ ] Generate CRC lookup table
- [ ] Implement address parsing
- [ ] Add digipeater handling

## Hardware Tasks

### Audio Input Circuit
- [ ] Design audio input circuit
- [ ] Calculate component values
- [ ] Build prototype on breadboard
- [ ] Test signal levels

### PCB Design
- [ ] Create schematic in KiCad
- [ ] Design PCB layout
- [ ] Order PCB
- [ ] Assemble and test

### Enclosure
- [ ] Design enclosure in CAD
- [ ] Cut/drill enclosure
- [ ] Mount components
- [ ] Label connectors

## Testing and Validation

### Unit Tests
- [ ] Test NRZI decoder
- [ ] Test bit stuffing
- [ ] Test CRC calculation
- [ ] Test address parsing

### Integration Tests
- [ ] End-to-end test with known packets
- [ ] Test with weak signals
- [ ] Test with noise
- [ ] Long-duration stability test

### Field Tests
- [ ] Test with local APRS traffic
- [ ] Test with digipeater
- [ ] Compare with commercial TNC
- [ ] Document real-world performance

## Documentation

- [x] Project README
- [x] AX.25 protocol documentation
- [x] Hardware design notes
- [x] FreeRTOS setup guide
- [ ] User manual
- [ ] Developer guide
- [ ] API documentation (Doxygen)
- [ ] Schematic and PCB files
