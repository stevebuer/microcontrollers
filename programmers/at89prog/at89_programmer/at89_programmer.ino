/*********************************************************************************************************/
/*                                       8051 PROGRAMMER (host-controlled)                                */
/*********************************************************************************************************/
/*
 * Based on the bit-banged AT89S51/AT89S52 ISP implementation by Syeed Mohd Ameen
 * (https://github.com/syeedameen/8051-programmer), MIT licensed.
 *
 * Changes in this version:
 *   1. FIXED addressing bug: the original `byte addrH = address & 0xff00;` truncated
 *      to zero because addrH is a single byte. Now correctly shifted down.
 *   2. Removed the hardcoded one-shot test-data programming in setup().
 *   3. Added a minimal binary serial command protocol so a host script (Python, or
 *      eventually a Makefile target) can drive programming instead of recompiling
 *      the sketch for every hex file.
 *
 * Wire protocol (host -> Arduino), all single-byte opcodes, host waits for response
 * before sending the next command:
 *
 *   0x01  PING       -> replies 0x50 ('P')
 *   0x02  ENABLE     -> asserts RST high, runs Programming Enable sequence.
 *                       replies 0x69 on success, 0x00 on failure (chip did not sync)
 *   0x03  ERASE      -> chip erase, waits DELAY_ERASE_MS, replies 0x06 (ACK)
 *   0x04  WRITE      -> host then sends 3 more bytes: addrH, addrL, data
 *                       replies 0x06 (ACK) after the byte is written
 *   0x05  READ       -> host then sends 2 more bytes: addrH, addrL
 *                       replies with the single data byte read back (no separate ACK)
 *   0x06  DISABLE    -> pulls RST low, replies 0x06 (ACK)
 *
 * This is intentionally minimal (no framing/checksum yet) to get something working
 * end to end. Add length-prefixed framing / CRC later if serial reliability becomes
 * an issue at longer cable runs or higher baud rates.
 */

/**************************************************************************************************************************************************************/
/*                                                          CONSTANTS DECLARATION                                                                             */
/**************************************************************************************************************************************************************/

/* spi bus pins definitions */
#define MISO 2
#define MOSI 3
#define SCK  4
#define RST  5

/* AT89S5x datasheet "don't care" flag */
#define DONT_CARE 0x00

/* Programming enable instruction */
#define PROGRAM_EN0 0xAC
#define PROGRAM_EN1 0x53
#define PROGRAM_EN2 DONT_CARE
#define PROGRAM_EN3 DONT_CARE

/* chip erase instruction */
#define CP_ERASE0 0xAC
#define CP_ERASE1 0x80
#define CP_ERASE2 DONT_CARE
#define CP_ERASE3 DONT_CARE

/* Read / write program memory instruction opcodes (byte mode) */
#define RD_BYTE0 0x20
#define WR_BYTE0 0x40

#define WRITE_LOCK_BITS_1 0xAC
#define WRITE_LOCK_BITS_2 0xE0
#define WRITE_LOCK_BITS_3 DONT_CARE
#define WRITE_LOCK_BITS_4 DONT_CARE

#define READ_LOCK_BITS_1 0x24
#define READ_LOCK_BITS_2 DONT_CARE
#define READ_LOCK_BITS_3 DONT_CARE
#define READ_LOCK_BITS_4 DONT_CARE

#define READ_SIG_BITS_1 0x28
#define READ_SIG_BITS_2 DONT_CARE
#define READ_SIG_BITS_3 DONT_CARE
#define READ_SIG_BITS_4 DONT_CARE

/* SCK half-period delay. Must satisfy SCK <= target crystal / 16.
 * Tuned here for a 12MHz target crystal against a 16MHz Arduino - adjust if your
 * target crystal changes. When in doubt, go slower, not faster. */
#define DELAY_INSTRUCTION_US 2

/* Chip erase requires >=500ms settle time per datasheet; padded for margin */
#define DELAY_ERASE_MS 512

/* Settle time after asserting RST high before sending Programming Enable */
#define DELAY_RESET_MS 10

/* Host protocol opcodes */
#define CMD_PING    0x01
#define CMD_ENABLE  0x02
#define CMD_ERASE   0x03
#define CMD_WRITE   0x04
#define CMD_READ    0x05
#define CMD_DISABLE 0x06

#define RESP_PING_OK 0x50   /* 'P' */
#define RESP_ACK     0x06
#define RESP_FAIL    0x00

/**************************************************************************************************************************************************************/
/*                                                              SETUP / LOOP                                                                                   */
/**************************************************************************************************************************************************************/

void setup()
{
  pinMode(SCK, OUTPUT);
  pinMode(RST, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(MISO, INPUT);

  digitalWrite(RST, LOW);   /* start deasserted; host explicitly asks to ENABLE */
  digitalWrite(SCK, LOW);

  Serial.begin(57600);
}

void loop()
{
  if (!Serial.available()) return;

  byte cmd = readByteBlocking();

  switch (cmd)
  {
    case CMD_PING:
      Serial.write(RESP_PING_OK);
      break;

    case CMD_ENABLE:
      handle_enable();
      break;

    case CMD_ERASE:
      chip_erase();
      Serial.write(RESP_ACK);
      break;

    case CMD_WRITE:
      handle_write();
      break;

    case CMD_READ:
      handle_read();
      break;

    case CMD_DISABLE:
      digitalWrite(RST, LOW);
      Serial.write(RESP_ACK);
      break;

    default:
      /* Unknown opcode: ignore silently so a stray byte doesn't wedge the protocol */
      break;
  }
}

/**************************************************************************************************************************************************************/
/*                                                          COMMAND HANDLERS                                                                                   */
/**************************************************************************************************************************************************************/

void handle_enable(void)
{
  digitalWrite(RST, HIGH);
  delay(DELAY_RESET_MS);
  digitalWrite(SCK, LOW);
  delay(DELAY_RESET_MS);

  byte result = program_enable();
  Serial.write(result);   /* 0x69 = success, anything else = failure/no sync */
}

void handle_write(void)
{
  byte addrH = readByteBlocking();
  byte addrL = readByteBlocking();
  byte data  = readByteBlocking();

  int address = (int(addrH) << 8) | addrL;
  write_byte(address, data);

  Serial.write(RESP_ACK);
}

void handle_read(void)
{
  byte addrH = readByteBlocking();
  byte addrL = readByteBlocking();

  int address = (int(addrH) << 8) | addrL;
  byte value = read_byte(address);

  Serial.write(value);
}

/* Blocking single-byte serial read. Simple and fine for a low-throughput ISP link;
 * revisit if you add framing/timeouts later. */
byte readByteBlocking(void)
{
  while (!Serial.available()) { /* spin */ }
  return Serial.read();
}

/**************************************************************************************************************************************************************/
/*                                                          ISP PRIMITIVES                                                                                     */
/*                                              (unchanged from original except addressing fix)                                                                */
/**************************************************************************************************************************************************************/

byte send_spi(byte instruction)
{
  byte bit_order;
  boolean flag;
  byte return_value = 0;

  for (byte i = 0; i < 8; i++)
  {
    flag = instruction & (0x80 >> i);
    digitalWrite(MOSI, flag);

    digitalWrite(SCK, true);
    delayMicroseconds(DELAY_INSTRUCTION_US);
    bit_order = digitalRead(MISO);
    digitalWrite(SCK, false);

    return_value += (bit_order << i) & 0xff;
  }
  return return_value;
}

byte program_enable(void)
{
  send_spi(PROGRAM_EN0);
  send_spi(PROGRAM_EN1);
  send_spi(PROGRAM_EN2);
  return send_spi(PROGRAM_EN3);
}

void chip_erase(void)
{
  send_spi(CP_ERASE0);
  send_spi(CP_ERASE1);
  send_spi(CP_ERASE2);
  send_spi(CP_ERASE3);
  delay(DELAY_ERASE_MS);
}

byte read_byte(int address)
{
  /* FIX: shift the high byte down instead of masking-and-truncating.
   * Original `byte addrH = address & 0xff00;` always evaluated to 0 because
   * assigning a 16-bit masked value into an 8-bit byte drops the high bits,
   * silently limiting addressing to 0x00-0xFF. */
  byte addrH = (address >> 8) & 0xff;
  byte addrL = address & 0xff;

  send_spi(RD_BYTE0);
  send_spi(addrH);
  send_spi(addrL);

  return send_spi(DONT_CARE);
}

void write_byte(int address, byte data)
{
  byte addrH = (address >> 8) & 0xff;   /* same fix as read_byte */
  byte addrL = address & 0xff;

  send_spi(WR_BYTE0);
  send_spi(addrH);
  send_spi(addrL);
  send_spi(data);
}

void write_lockbits(byte bits)
{
  send_spi(WRITE_LOCK_BITS_1);
  send_spi(WRITE_LOCK_BITS_2);
  send_spi(WRITE_LOCK_BITS_3 | (bits & 0x03));
  send_spi(WRITE_LOCK_BITS_4);
}

byte read_lockbits(void)
{
  send_spi(READ_LOCK_BITS_1);
  send_spi(READ_LOCK_BITS_2);
  send_spi(READ_LOCK_BITS_3);
  byte temp = send_spi(READ_LOCK_BITS_4);

  temp = temp >> 2;
  temp = temp & 0x03;

  return temp;
}

byte read_signature(byte address)
{
  send_spi(READ_SIG_BITS_1);
  send_spi(READ_SIG_BITS_2 | (address >> 1));
  send_spi(READ_SIG_BITS_3 | (address << 7));
  return send_spi(READ_SIG_BITS_4);
}
