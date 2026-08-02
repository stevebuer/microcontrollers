#!/usr/bin/env python3

"""Read a Bosch BMP180 sensor through an FT232H using Adafruit Blinka."""

import argparse
import math
import os
import struct
import sys
import time


BMP180_ADDR = 0x77
BMP180_CHIP_ID = 0x55
REG_CHIP_ID = 0xD0
REG_CONTROL = 0xF4
REG_RESULT = 0xF6
REG_CALIBRATION = 0xAA
CMD_READ_TEMP = 0x2E
CMD_READ_PRESSURE = 0x34
PRESSURE_DELAYS = {
	0: 0.005,
	1: 0.008,
	2: 0.014,
	3: 0.026,
}


def parse_args() -> argparse.Namespace:
	parser = argparse.ArgumentParser(
		description="Read temperature and pressure from a BMP180 over FT232H Blinka I2C."
	)
	parser.add_argument(
		"-o",
		"--oversampling",
		type=int,
		choices=range(4),
		default=3,
		help="BMP180 oversampling setting from 0 to 3",
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
		default=101325.0,
		metavar="PA",
		help="reference sea-level pressure in pascals for altitude estimates",
	)
	parser.add_argument(
		"--dump-calibration",
		action="store_true",
		help="print the BMP180 calibration words before reading measurements",
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


class BMP180:
	def __init__(self, i2c, oversampling: int = 3):
		self.i2c = i2c
		self.address = BMP180_ADDR
		self.oversampling = oversampling
		self.calibration = {}
		self._check_presence()
		self._check_chip_id()
		self._read_calibration()

	def _check_presence(self) -> None:
		devices = scan_i2c(self.i2c)
		if self.address not in devices:
			print(f"BMP180 not found at 0x{self.address:02X}.", file=sys.stderr)
			print("Run the FT232H I2C scanner first to confirm bus wiring.", file=sys.stderr)
			raise SystemExit(1)

	def _write_then_read(self, register: int, length: int) -> bytes:
		result = bytearray(length)
		self.i2c.writeto(self.address, bytes([register]))
		self.i2c.readfrom_into(self.address, result)
		return bytes(result)

	def _read_u8(self, register: int) -> int:
		return self._write_then_read(register, 1)[0]

	def _read_u16(self, register: int) -> int:
		return struct.unpack(">H", self._write_then_read(register, 2))[0]

	def _read_s16(self, register: int) -> int:
		return struct.unpack(">h", self._write_then_read(register, 2))[0]

	def _check_chip_id(self) -> None:
		chip_id = self._read_u8(REG_CHIP_ID)
		if chip_id != BMP180_CHIP_ID:
			print(f"Unexpected chip ID 0x{chip_id:02X}; expected 0x{BMP180_CHIP_ID:02X}.", file=sys.stderr)
			raise SystemExit(1)

	def _read_calibration(self) -> None:
		self.calibration = {
			"ac1": self._read_s16(0xAA),
			"ac2": self._read_s16(0xAC),
			"ac3": self._read_s16(0xAE),
			"ac4": self._read_u16(0xB0),
			"ac5": self._read_u16(0xB2),
			"ac6": self._read_u16(0xB4),
			"b1": self._read_s16(0xB6),
			"b2": self._read_s16(0xB8),
			"mb": self._read_s16(0xBA),
			"mc": self._read_s16(0xBC),
			"md": self._read_s16(0xBE),
		}

	def read_raw_temperature(self) -> int:
		self.i2c.writeto(self.address, bytes([REG_CONTROL, CMD_READ_TEMP]))
		time.sleep(0.005)
		return self._read_u16(REG_RESULT)

	def read_raw_pressure(self) -> int:
		command = CMD_READ_PRESSURE + (self.oversampling << 6)
		self.i2c.writeto(self.address, bytes([REG_CONTROL, command]))
		time.sleep(PRESSURE_DELAYS[self.oversampling])
		raw = self._write_then_read(REG_RESULT, 3)
		value = (raw[0] << 16) | (raw[1] << 8) | raw[2]
		return value >> (8 - self.oversampling)

	def read_measurement(self) -> dict[str, float]:
		raw_temp = self.read_raw_temperature()
		raw_pressure = self.read_raw_pressure()

		ac1 = self.calibration["ac1"]
		ac2 = self.calibration["ac2"]
		ac3 = self.calibration["ac3"]
		ac4 = self.calibration["ac4"]
		ac5 = self.calibration["ac5"]
		ac6 = self.calibration["ac6"]
		b1 = self.calibration["b1"]
		b2 = self.calibration["b2"]
		mc = self.calibration["mc"]
		md = self.calibration["md"]

		x1 = ((raw_temp - ac6) * ac5) >> 15
		x2 = (mc << 11) // (x1 + md)
		b5 = x1 + x2
		temperature_c = ((b5 + 8) >> 4) / 10.0

		b6 = b5 - 4000
		x1 = (b2 * ((b6 * b6) >> 12)) >> 11
		x2 = (ac2 * b6) >> 11
		x3 = x1 + x2
		b3 = ((((ac1 * 4) + x3) << self.oversampling) + 2) >> 2

		x1 = (ac3 * b6) >> 13
		x2 = (b1 * ((b6 * b6) >> 12)) >> 16
		x3 = (x1 + x2 + 2) >> 2
		b4 = (ac4 * (x3 + 32768)) >> 15
		b7 = (raw_pressure - b3) * (50000 >> self.oversampling)

		if b7 < 0x80000000:
			pressure = (b7 * 2) // b4
		else:
			pressure = (b7 // b4) * 2

		x1 = (pressure >> 8) * (pressure >> 8)
		x1 = (x1 * 3038) >> 16
		x2 = (-7357 * pressure) >> 16
		pressure += (x1 + x2 + 3791) >> 4

		return {
			"temperature_c": temperature_c,
			"temperature_f": (temperature_c * 9.0 / 5.0) + 32.0,
			"pressure_pa": float(pressure),
			"raw_temperature": float(raw_temp),
			"raw_pressure": float(raw_pressure),
		}


def scan_i2c(i2c) -> list[int]:
	while not i2c.try_lock():
		time.sleep(0.05)

	try:
		return list(i2c.scan())
	finally:
		i2c.unlock()


def pressure_to_altitude_meters(pressure_pa: float, sea_level_pa: float) -> float:
	return 44330.0 * (1.0 - math.pow(pressure_pa / sea_level_pa, 0.1903))


def print_calibration(calibration: dict[str, int]) -> None:
	print("Calibration words:")
	for name, value in calibration.items():
		print(f"  {name.upper():>3} = {value}")


def print_measurement(measurement: dict[str, float], sea_level_pressure: float) -> None:
	altitude_m = pressure_to_altitude_meters(
		measurement["pressure_pa"], sea_level_pressure
	)
	print(
		"Temperature: "
		f"{measurement['temperature_c']:.1f} C / {measurement['temperature_f']:.1f} F"
	)
	print(
		"Pressure: "
		f"{measurement['pressure_pa']:.0f} Pa / {measurement['pressure_pa'] / 100.0:.2f} hPa"
	)
	print(f"Altitude: {altitude_m:.1f} m")
	print(
		"Raw values: "
		f"UT={int(measurement['raw_temperature'])} UP={int(measurement['raw_pressure'])}"
	)


def main() -> int:
	args = parse_args()
	require_ft232h_env()
	i2c = open_i2c()

	try:
		sensor = BMP180(i2c, oversampling=args.oversampling)
		print(f"Found BMP180 at 0x{sensor.address:02X}")

		if args.dump_calibration:
			print_calibration(sensor.calibration)

		while True:
			print_measurement(
				sensor.read_measurement(),
				sea_level_pressure=args.sea_level_pressure,
			)

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
