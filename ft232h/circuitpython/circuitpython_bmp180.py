#!/usr/bin/env python3

"""Read a BMP180 over FT232H using the CircuitPython BMP180 library."""

import argparse
import os
import sys
import time


MODE_NAMES = {
	"ultralowpower": "MODE_ULTRALOWPOWER",
	"standard": "MODE_STANDARD",
	"highres": "MODE_HIGHRES",
	"ultrahighres": "MODE_ULTRAHIGHRES",
}


def parse_args() -> argparse.Namespace:
	parser = argparse.ArgumentParser(
		description="Read temperature and pressure from a BMP180 using CircuitPython."
	)
	parser.add_argument(
		"-m",
		"--mode",
		choices=tuple(MODE_NAMES),
		default="ultrahighres",
		help="BMP180 measurement mode",
	)
	parser.add_argument(
		"-r",
		"--repeat",
		type=float,
		default=0,
		metavar="SECONDS",
		help="repeat the measurement every N seconds",
	)
	parser.add_argument(
		"-s",
		"--sea-level-pressure",
		type=float,
		default=1013.25,
		metavar="HPA",
		help="reference sea-level pressure in hPa for altitude estimates",
	)
	parser.add_argument(
		"--dump-coefficients",
		action="store_true",
		help="print the calibration coefficients exposed by the library",
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
		import bmp180
	except ImportError as exc:
		print("Missing CircuitPython BMP180 dependencies.", file=sys.stderr)
		print(
			"Install the packages from ft232h/requirements.txt in your virtual environment.",
			file=sys.stderr,
		)
		raise SystemExit(2) from exc

	return board, bmp180


def open_sensor(mode_name: str, sea_level_pressure: float):
	board, bmp180 = import_dependencies()

	try:
		i2c = board.I2C()
		sensor = bmp180.BMP180(i2c)
	except Exception as exc:  # hardware/backend errors vary by platform
		print("Unable to initialize the FT232H BMP180 interface.", file=sys.stderr)
		print(
			"Check the USB connection, udev permissions, BLINKA_FT232H=1, and BMP180 wiring.",
			file=sys.stderr,
		)
		raise SystemExit(1) from exc

	sensor.mode = getattr(bmp180, MODE_NAMES[mode_name])
	sensor.sea_level_pressure = sea_level_pressure
	return i2c, sensor


def print_coefficients(sensor) -> None:
	names = ("AC1", "AC2", "AC3", "AC4", "AC5", "AC6", "B1", "B2", "MB", "MC", "MD")
	print("Calibration coefficients:")
	for name, value in zip(names, sensor.coeffs_mem):
		print(f"  {name:>3} = {value}")


def print_measurement(sensor) -> None:
	temperature_c = sensor.temperature
	pressure_hpa = sensor.pressure
	altitude_m = sensor.altitude

	print(f"Temperature: {temperature_c:.1f} C / {(temperature_c * 9.0 / 5.0) + 32.0:.1f} F")
	print(f"Pressure: {pressure_hpa:.2f} hPa / {pressure_hpa * 100.0:.0f} Pa")
	print(f"Altitude: {altitude_m:.1f} m")


def main() -> int:
	args = parse_args()
	require_ft232h_env()
	i2c, sensor = open_sensor(args.mode, args.sea_level_pressure)

	try:
		print("BMP180 initialized through CircuitPython library")

		if args.dump_coefficients:
			print_coefficients(sensor)

		while True:
			print_measurement(sensor)

			if args.repeat <= 0:
				break

			print()
			time.sleep(args.repeat)
	finally:
		deinit = getattr(i2c, "deinit", None)
		if callable(deinit):
			deinit()

	return 0


if __name__ == "__main__":
	raise SystemExit(main())
