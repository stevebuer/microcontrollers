#!/usr/bin/env python3

"""Read a DHT11 sensor using CircuitPython on FT232H/Blinka."""

import argparse
import importlib
import json
import os
import sys
import time
from datetime import datetime, timezone


def parse_args() -> argparse.Namespace:
	parser = argparse.ArgumentParser(
		description="Read temperature and humidity from a DHT11 sensor."
	)
	parser.add_argument(
		"--pin",
		default="C0",
		help="FT232H pin name on board module (default: C0)",
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
		adafruit_dht = importlib.import_module("adafruit_dht")
	except ImportError as exc:
		print("Missing CircuitPython DHT dependencies.", file=sys.stderr)
		print(
			"Install packages from ft232h/requirements.txt in your virtual environment.",
			file=sys.stderr,
		)
		raise SystemExit(2) from exc

	return board, adafruit_dht


def open_sensor(pin_name: str):
	board, adafruit_dht = import_dependencies()

	pin = getattr(board, pin_name, None)
	if pin is None:
		print(f"Unknown board pin '{pin_name}'.", file=sys.stderr)
		raise SystemExit(2)

	try:
		# use_pulseio=False keeps this portable for Linux host adapters.
		return adafruit_dht.DHT11(pin, use_pulseio=False)
	except Exception as exc:
		print("Unable to initialize DHT11 sensor.", file=sys.stderr)
		print("Check wiring, selected pin, and sensor power.", file=sys.stderr)
		raise SystemExit(1) from exc


def read_once(sensor) -> tuple[float, float]:
	# DHT reads can intermittently fail; caller handles retries/continuation.
	temperature_c = sensor.temperature
	humidity = sensor.humidity
	if temperature_c is None or humidity is None:
		raise RuntimeError("No sample returned by DHT11")
	return float(temperature_c), float(humidity)


def emit_human(temperature_c: float, humidity: float) -> None:
	temperature_f = temperature_c * 9.0 / 5.0 + 32.0
	print(f"Temperature: {temperature_c:.1f} C / {temperature_f:.1f} F")
	print(f"Humidity: {humidity:.1f} %")


def utc_timestamp() -> str:
	return datetime.now(timezone.utc).isoformat(timespec="seconds").replace("+00:00", "Z")


def emit_json(temperature_c: float, humidity: float) -> None:
	payload = {
		"timestamp_utc": utc_timestamp(),
		"temperature_c": round(temperature_c, 1),
		"temperature_f": round(temperature_c * 9.0 / 5.0 + 32.0, 1),
		"humidity_pct": round(humidity, 1),
	}
	print(json.dumps(payload, separators=(",", ":")))


def main() -> int:
	args = parse_args()
	require_ft232h_env()
	sensor = open_sensor(args.pin)

	try:
		while True:
			try:
				temperature_c, humidity = read_once(sensor)
			except Exception as exc:
				print(f"Read failed: {exc}", file=sys.stderr)
				if args.repeat <= 0:
					return 1
				time.sleep(args.repeat)
				continue

			if args.json:
				emit_json(temperature_c, humidity)
			else:
				emit_human(temperature_c, humidity)

			if args.repeat <= 0:
				break

			time.sleep(args.repeat)
	finally:
		exit_method = getattr(sensor, "exit", None)
		if callable(exit_method):
			exit_method()

	return 0


if __name__ == "__main__":
	raise SystemExit(main())
