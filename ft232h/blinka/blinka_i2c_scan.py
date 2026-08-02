#!/usr/bin/env python3

"""Scan the FT232H I2C bus using Adafruit Blinka."""

import argparse
import os
import sys
import time


def parse_args() -> argparse.Namespace:
	parser = argparse.ArgumentParser(
		description="Scan for I2C devices attached to an FT232H running Blinka."
	)
	parser.add_argument(
		"-r",
		"--repeat",
		type=float,
		default=0,
		metavar="SECONDS",
		help="repeat the scan every N seconds",
	)
	return parser.parse_args()


def require_ft232h_env() -> None:
	if os.environ.get("BLINKA_FT232H") == "1":
		return

	print("BLINKA_FT232H is not set to 1.", file=sys.stderr)
	print("Export BLINKA_FT232H=1 before running this script.", file=sys.stderr)
	raise SystemExit(2)


def open_i2c():
	try:
		import board
		import busio
	except ImportError as exc:
		print("Missing Blinka dependencies.", file=sys.stderr)
		print(
			"Install the packages from ft232h/requirements.txt in your virtual environment.",
			file=sys.stderr,
		)
		raise SystemExit(2) from exc

	try:
		return busio.I2C(board.SCL, board.SDA)
	except Exception as exc:  # hardware/backend errors vary by platform
		print("Unable to initialize the FT232H I2C interface.", file=sys.stderr)
		print(
			"Check the USB connection, udev permissions, BLINKA_FT232H=1, and SDA/SCL wiring.",
			file=sys.stderr,
		)
		raise SystemExit(1) from exc


def scan_once(i2c) -> list[int]:
	while not i2c.try_lock():
		time.sleep(0.05)

	try:
		return list(i2c.scan())
	finally:
		i2c.unlock()


def format_addresses(addresses: list[int]) -> str:
	return ", ".join(f"0x{address:02X} ({address})" for address in addresses)


def main() -> int:
	args = parse_args()
	require_ft232h_env()
	i2c = open_i2c()

	try:
		while True:
			devices = scan_once(i2c)
			if devices:
				print(f"Found {len(devices)} device(s): {format_addresses(devices)}")
			else:
				print("No I2C devices found.")

			if args.repeat <= 0:
				break

			time.sleep(args.repeat)
	finally:
		deinit = getattr(i2c, "deinit", None)
		if callable(deinit):
			deinit()

	return 0


if __name__ == "__main__":
	raise SystemExit(main())
