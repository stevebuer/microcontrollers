#!/usr/bin/env python3

"""Read DS18B20 sensors using CircuitPython on FT232H/Blinka."""

import argparse
import importlib
import json
import os
import sys
import time
from datetime import datetime, timezone


def parse_args() -> argparse.Namespace:
	parser = argparse.ArgumentParser(
		description="Scan and read DS18B20 temperature sensors on a 1-Wire bus."
	)
	parser.add_argument(
		"--pin",
		default="C0",
		help="FT232H pin name on board module (default: C0)",
	)
	parser.add_argument(
		"--address",
		default="",
		help="optional DS18B20 ROM address to select (hex, with or without separators)",
	)
	parser.add_argument(
		"--list-only",
		action="store_true",
		help="scan and list detected ROM addresses without reading temperatures",
	)
	parser.add_argument(
		"-r",
		"--repeat",
		type=float,
		default=0,
		metavar="SECONDS",
		help="repeat interval in seconds (default: one-shot)",
	)
	parser.add_argument(
		"--json",
		action="store_true",
		help="emit one-line JSON for easy machine parsing",
	)
	return parser.parse_args()


def require_ft232h_env() -> None:
	if os.environ.get("BLINKA_FT232H") == "1":
		return

	print("BLINKA_FT232H is not set to 1.", file=sys.stderr)
	print("Export BLINKA_FT232H=1 before running this script.", file=sys.stderr)
	raise SystemExit(2)


def import_dependencies():
	try:
		import board
		one_wire_mod = importlib.import_module("adafruit_onewire.bus")
		ds18_mod = importlib.import_module("adafruit_ds18x20")
	except ImportError as exc:
		print("Missing DS18B20 CircuitPython dependencies.", file=sys.stderr)
		print(
			"Install packages from ft232h/requirements.txt in your virtual environment.",
			file=sys.stderr,
		)
		raise SystemExit(2) from exc

	return board, one_wire_mod.OneWireBus, ds18_mod.DS18X20


def normalize_rom(text: str) -> str:
	cleaned = "".join(ch for ch in text.upper() if ch in "0123456789ABCDEF")
	return cleaned


def rom_to_hex(rom: bytes) -> str:
	return "".join(f"{value:02X}" for value in rom)


def utc_timestamp() -> str:
	return datetime.now(timezone.utc).isoformat(timespec="seconds").replace("+00:00", "Z")


def select_devices(roms: list[bytes], requested_rom: str) -> list[bytes]:
	if not requested_rom:
		return roms

	normalized_requested = normalize_rom(requested_rom)
	selected = [rom for rom in roms if rom_to_hex(rom) == normalized_requested]
	return selected


def emit_list(roms: list[bytes], json_mode: bool) -> None:
	addresses = [rom_to_hex(rom) for rom in roms]
	if json_mode:
		print(json.dumps({"timestamp_utc": utc_timestamp(), "devices": addresses}, separators=(",", ":")))
		return

	if not addresses:
		print("No 1-Wire devices found.")
		return

	print("Detected 1-Wire devices:")
	for addr in addresses:
		print(f"  {addr}")


def emit_readings(readings: list[dict], json_mode: bool) -> None:
	if json_mode:
		print(json.dumps({"timestamp_utc": utc_timestamp(), "readings": readings}, separators=(",", ":")))
		return

	if not readings:
		print("No DS18B20 devices selected.")
		return

	for item in readings:
		temp_c = item["temperature_c"]
		temp_f = temp_c * 9.0 / 5.0 + 32.0
		print(f"{item['address']}: {temp_c:.2f} C / {temp_f:.2f} F")


def main() -> int:
	args = parse_args()
	require_ft232h_env()
	board, one_wire_bus_class, ds18x20_class = import_dependencies()

	pin = getattr(board, args.pin, None)
	if pin is None:
		print(f"Unknown board pin '{args.pin}'.", file=sys.stderr)
		return 2

	try:
		one_wire_bus = one_wire_bus_class(pin)
	except Exception as exc:
		print("Unable to initialize 1-Wire bus.", file=sys.stderr)
		print("Check wiring, selected pin, and pull-up resistor.", file=sys.stderr)
		print(f"Details: {exc}", file=sys.stderr)
		return 1

	while True:
		try:
			roms = [bytes(rom) for rom in one_wire_bus.scan()]
		except Exception as exc:
			print(f"1-Wire scan failed: {exc}", file=sys.stderr)
			if args.repeat <= 0:
				return 1
			time.sleep(args.repeat)
			continue

		if args.list_only:
			emit_list(roms, args.json)
			if args.repeat <= 0:
				return 0
			time.sleep(args.repeat)
			continue

		selected_roms = select_devices(roms, args.address)
		if args.address and not selected_roms:
			print("Requested DS18B20 address not found on bus.", file=sys.stderr)
			if args.repeat <= 0:
				return 1
			time.sleep(args.repeat)
			continue

		readings = []
		for rom in selected_roms:
			try:
				sensor = ds18x20_class(one_wire_bus, rom)
				temp_c = float(sensor.temperature)
				readings.append(
					{
						"address": rom_to_hex(rom),
						"temperature_c": round(temp_c, 2),
						"temperature_f": round(temp_c * 9.0 / 5.0 + 32.0, 2),
					}
				)
			except Exception as exc:
				print(f"Read failed for {rom_to_hex(rom)}: {exc}", file=sys.stderr)

		emit_readings(readings, args.json)

		if args.repeat <= 0:
			return 0 if readings else 1

		time.sleep(args.repeat)


if __name__ == "__main__":
	raise SystemExit(main())
