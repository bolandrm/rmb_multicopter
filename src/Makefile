ARDUINO_DIR   = /Applications/Arduino.app/Contents/Resources/Java
USER_LIB_PATH = ./libraries
OBJDIR        = ./.build-$(BOARD_TAG)

BOARD_TAG    = teensy31
ARDUINO_LIBS = i2c_t3 MedianFilter

MONITOR_BAUDRATE  = 115200
MONITOR_PORT      = /dev/tty.usbmodem*

NO_CORE_MAIN_CPP = true

# for additional directories
# LOCAL_CPP_SRCS = $(wildcard *.cpp) $(wildcard ./hardware/*.cpp)
# CPPFLAGS = -I./hardware -I.

CFLAGS += -std=c99

include ~/hardware/Arduino-Makefile/Teensy.mk

TEENSY_LOADER_CLI = ~/hardware/teensy_loader_cli/teensy_loader_cli
REBOOT_TEENSY = stty -f $(MONITOR_PORT) 134

burn:
	make
	if [ -a $(MONITOR_PORT) ]; then $(REBOOT_TEENSY); fi;
	$(TEENSY_LOADER_CLI) -w -v -mmcu=mk20dx256 .build-teensy31/src.hex

burnm:
	make burn && sleep 1 && make monitor

make check:
	cppcheck --enable=all --inconclusive --quiet $(LOCAL_C_SRCS) $(LOCAL_CPP_SRCS)
