ARDUINO_DIR   = /Applications/Arduino.app/Contents/Resources/Java
USER_LIB_PATH = ./../libraries
OBJDIR        = ./.build-$(BOARD_TAG)

BOARD_TAG    = teensy31
ARDUINO_LIBS = Wire

MONITOR_BAUDRATE  = 115200
MONITOR_PORT      = /dev/tty.usbmodem*
#MONITOR_PORT      = /dev/tty.usbserial
#MONITOR_PORT      = /dev/cu.SLAB_USBtoUART

include ~/hardware/Arduino-Makefile/Teensy.mk

up:
	make
	~/hardware/teensy-loader-cli/teensy_loader_cli -w -v -mmcu=mk20dx256 .build-teensy31/src.hex
