FQBN     := arduino:avr:uno
PORT     := /dev/ttyACM0
SKETCH   := cw_beacon

.PHONY: all compile upload flash monitor clean

all: flash

compile:
	arduino-cli compile --fqbn $(FQBN) $(SKETCH)

upload:
	arduino-cli upload -p $(PORT) --fqbn $(FQBN) $(SKETCH)

flash: compile upload

monitor:
	arduino-cli monitor -p $(PORT) -c baudrate=57600

clean:
	rm -rf $(SKETCH)/build
