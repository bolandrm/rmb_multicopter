ARDUINO_DIR   = /Applications/Arduino.app/Contents/Java
USER_LIB_PATH = ./libraries
OBJDIR        = ./.build-$(BOARD_TAG)

BOARD_TAG    = teensy31
ARDUINO_LIBS = i2c_t3 MedianFilter EEPROM SPI SD

F_CPU=120000000

OPTIMIZATION_LEVEL = 1

MONITOR_BAUDRATE  = 115200
MONITOR_PORT      = /dev/tty.usbmodem*

NO_CORE_MAIN_CPP = true

# for additional directories
# LOCAL_CPP_SRCS = $(wildcard *.cpp) $(wildcard ./hardware/*.cpp)
# CPPFLAGS = -I./hardware -I.

CFLAGS += -std=c99 -D__MK20DX256__ -DTEENSYDUINO=128
CPPFLAGS += -D__MK20DX256__ -DTEENSYDUINO=128

include ../build_scripts/Teensy.mk

TEENSY_LOADER_CLI = ../teensy_loader_cli/teensy_loader_cli
REBOOT_TEENSY = stty -f $(MONITOR_PORT) 134

burn:
	make
	if [ -a $(MONITOR_PORT) ]; then $(REBOOT_TEENSY); fi;
	$(TEENSY_LOADER_CLI) -w -v -mmcu=mk20dx256 .build-teensy31/src.hex

burnm:
	make burn && sleep 1 && make monitor

make check:
	cppcheck --enable=all --inconclusive --quiet $(LOCAL_C_SRCS) $(LOCAL_CPP_SRCS)
