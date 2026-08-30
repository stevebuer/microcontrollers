#!/usr/bin/env python3
"""
Minimal end-to-end test for the at89_programmer sketch's serial protocol.

Round-trip: PING -> ENABLE -> ERASE -> WRITE(addr 0, 0xA5) -> READ(addr 0)

Usage:
    python3 test_programmer.py --port /dev/ttyACM0
"""

import argparse
import serial
import sys
import time

CMD_PING    = 0x01
CMD_ENABLE  = 0x02
CMD_ERASE   = 0x03
CMD_WRITE   = 0x04
CMD_READ    = 0x05
CMD_DISABLE = 0x06

RESP_PING_OK = 0x50  # 'P'
RESP_ACK     = 0x06
ENABLE_OK    = 0x69

BAUD = 57600


def open_port(port):
    ser = serial.Serial(port, BAUD, timeout=3)
    # Arduino resets on DTR toggle when the port opens; give it time to boot
    # and flush whatever boot noise might be sitting in the buffer.
    time.sleep(2)
    ser.reset_input_buffer()
    return ser


def expect_byte(ser, expected, what):
    resp = ser.read(1)
    if not resp:
        sys.exit(f"FAIL: no response for {what} (timeout)")
    got = resp[0]
    if got != expected:
        sys.exit(f"FAIL: {what} expected {expected:#04x}, got {got:#04x}")
    print(f"OK:   {what} -> {got:#04x}")
    return got


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--port", required=True)
    args = ap.parse_args()

    ser = open_port(args.port)

    print("Sending PING...")
    ser.write(bytes([CMD_PING]))
    expect_byte(ser, RESP_PING_OK, "PING")

    print("Sending ENABLE...")
    ser.write(bytes([CMD_ENABLE]))
    resp = ser.read(1)
    if not resp or resp[0] != ENABLE_OK:
        sys.exit(f"FAIL: ENABLE did not sync (got {resp!r}). "
                  f"Check wiring/RST/SCK timing before continuing.")
    print(f"OK:   ENABLE -> {resp[0]:#04x} (chip synced)")

    print("Sending ERASE (this takes ~500ms)...")
    ser.write(bytes([CMD_ERASE]))
    expect_byte(ser, RESP_ACK, "ERASE")

    test_addr = 0x0000
    test_data = 0xA5

    print(f"Writing {test_data:#04x} to address {test_addr:#06x}...")
    ser.write(bytes([CMD_WRITE, (test_addr >> 8) & 0xFF, test_addr & 0xFF, test_data]))
    expect_byte(ser, RESP_ACK, "WRITE")

    print(f"Reading back address {test_addr:#06x}...")
    ser.write(bytes([CMD_READ, (test_addr >> 8) & 0xFF, test_addr & 0xFF]))
    resp = ser.read(1)
    if not resp:
        sys.exit("FAIL: no response for READ (timeout)")
    readback = resp[0]
    if readback != test_data:
        sys.exit(f"FAIL: verify mismatch. Wrote {test_data:#04x}, read back {readback:#04x}")
    print(f"OK:   READ -> {readback:#04x} (matches what was written)")

    print("Sending DISABLE...")
    ser.write(bytes([CMD_DISABLE]))
    expect_byte(ser, RESP_ACK, "DISABLE")

    print("\nAll steps passed. ISP core is working end to end.")


if __name__ == "__main__":
    main()
