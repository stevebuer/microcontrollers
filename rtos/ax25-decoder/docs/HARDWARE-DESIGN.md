# Hardware Design Notes for AX.25 Decoder

## Audio Input Circuit

### Requirements
- Convert radio audio output to STM32 ADC-compatible signal
- Voltage range: 0-3.3V (STM32 ADC input)
- Frequency response: 300 Hz - 3 kHz
- Input impedance: ~10 kΩ (typical radio headphone/speaker output)

### Circuit Design

```
Radio Audio Output (typically 100mV - 1V AC)
    |
    +--- C1 (10µF) --- R1 (10kΩ) --- R2 (10kΩ) --- ADC Input (PA0)
                           |                |
                          GND              C2 (100nF)
                                            |
                                           GND
```

#### Component Values
- **C1**: 10 µF (AC coupling, blocks DC)
- **R1, R2**: 10 kΩ (voltage divider, biases signal to 1.65V)
- **C2**: 100 nF (low-pass filter, removes RF/noise)

#### Optional Op-Amp Stage
For weak signals, add an op-amp buffer/amplifier:
- Use rail-to-rail op-amp (e.g., MCP6001, TLV2371)
- Gain: 2-10× adjustable
- Power from STM32 3.3V rail

### ADC Configuration
- **Resolution**: 12-bit (0-4095)
- **Sample Rate**: 9600 Hz minimum (recommend 19200 Hz)
- **Timer**: TIM2 triggers ADC conversion
- **DMA**: Use DMA1 for automatic buffer filling
- **Reference**: Internal 3.3V (VDDA)

## Input Protection

### ESD Protection
- Add Schottky diodes to clamp voltage to 0-3.3V range
- TVS diode for transient protection

### Signal Levels
| Radio Output | After Divider | ADC Reading |
|--------------|---------------|-------------|
| 0V (DC bias) | 1.65V         | 2048        |
| 1Vpp         | 1.15-2.15V    | 1425-2671   |
| 2Vpp         | 0.65-2.65V    | 800-3296    |

## Display Options

### 1. OLED Display (128x64, I2C)
- **Interface**: I2C (SDA=PB7, SCL=PB6)
- **Display**: Decoded packets, statistics
- **Library**: u8g2 or Adafruit SSD1306

### 2. Serial Terminal (UART)
- **Interface**: UART1 (TX=PA9, RX=PA10)
- **Baud**: 115200
- **Output**: Decoded frames in text format

### 3. LED Status Indicators
- **PC13**: Built-in LED (heartbeat)
- **PA1**: Valid packet indicator
- **PA2**: Sync/Lock indicator

## SD Card Logging (Optional)

### SD Card Interface
- **Protocol**: SPI
- **Pins**: 
  - SCK = PA5
  - MISO = PA6
  - MOSI = PA7
  - CS = PA4

### Data Format
Store decoded packets in CSV format:
```
Timestamp, Source, Destination, Path, Info
12345678, N0CALL-1, APRS, WIDE1-1,WIDE2-1, !4903.50N/07201.75W-
```

## Power Supply

### Power Options
1. **USB**: 5V from USB, regulated to 3.3V on-board
2. **Battery**: 3.7V LiPo with LDO regulator
3. **Radio**: Power from radio accessory port (typically 8-13.8V)

### Current Budget
| Component | Current | Notes |
|-----------|---------|-------|
| STM32F103 | 30-50 mA | At 72 MHz |
| OLED Display | 10-20 mA | When active |
| SD Card | 50-100 mA | During write |
| **Total** | **90-170 mA** | |

## PCB Layout Considerations

### Critical Traces
1. **ADC Input**: Keep short, avoid crossing digital lines
2. **Crystal**: Guard rings around 8 MHz crystal
3. **Power**: Star ground for analog and digital

### Recommended Layer Stack (2-layer)
- **Top**: Components, signal traces
- **Bottom**: Ground plane, power traces

## Enclosure

### Connectors
- **Audio In**: 3.5mm jack (mono or stereo)
- **USB**: Micro-USB or USB-C
- **Programming**: SWD header (4-pin: GND, 3V3, SWDIO, SWCLK)

### Dimensions
- Target: Hammond 1593K (66x66x28mm)
- Cutouts: USB, Audio jack, OLED display

## Testing Setup

### Required Equipment
1. **Radio**: VHF/UHF with packet capability
2. **Signal Generator**: For testing AFSK (or use PC + audio software)
3. **Oscilloscope**: Verify signal levels and timing
4. **Logic Analyzer**: Debug I2C, SPI, UART

### Test Signals
Generate test AFSK with:
- **Direwolf**: Software TNC
- **Audacity**: Generate 1200/2200 Hz tones
- **Function Generator**: If available

## Bill of Materials (Estimated)

| Part | Qty | Est. Cost |
|------|-----|-----------|
| STM32F103C8T6 Blue Pill | 1 | $3-5 |
| 0.96" OLED Display | 1 | $3-5 |
| Resistors, Capacitors | ~10 | $1 |
| Audio Jack | 1 | $0.50 |
| Enclosure | 1 | $5-10 |
| PCB (custom) | 1 | $5-15 |
| **Total** | | **$17-36** |

## Future Enhancements

### Hardware
- [ ] Add external RTC for timestamping
- [ ] GPS module for APRS digipeater
- [ ] PTT control for transmit capability
- [ ] Separate transmit/receive paths

### Features
- [ ] Dual channel decoder (VHF + UHF)
- [ ] Selectable filter bandwidth
- [ ] AGC (Automatic Gain Control)
- [ ] Audio pass-through
