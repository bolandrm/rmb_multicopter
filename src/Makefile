ARDUINO_DIR   = /Applications/Arduino.app/Contents/Resources/Java
USER_LIB_PATH = ./libraries
OBJDIR        = ./.build-$(BOARD_TAG)
AVR_TOOLS_PATH = ~/hardware/gcc-arm-none-eabi-4_9-2014q4/bin

BOARD_TAG    = teensy31
ARDUINO_LIBS = MedianFilter i2c_t3

MONITOR_BAUDRATE  = 115200
MONITOR_PORT      = /dev/tty.usbmodem*
#MONITOR_PORT      = /dev/tty.usbserial
#MONITOR_PORT      = /dev/cu.SLAB_USBtoUART

#~/hardware/teensy-loader-cli/teensy_loader_cli -w -v -mmcu=mk20dx256 build/main.hex

LOCAL_CPP_SRCS = $(wildcard *.cpp) $(wildcard ./hardware/*.cpp)
CPPFLAGS = -I./hardware -I. 

CXX_NAME     = arm-none-eabi-gcc

include ~/hardware/Arduino-Makefile/Teensy.mk

LDFLAGS2 = $(filter-out -larm_cortexM4l_math,$(LDFLAGS))

$(TARGET_ELF): 	$(LOCAL_OBJS) $(CORE_LIB) $(OTHER_OBJS)
		$(CC) $(LDFLAGS2) -o $@ $(LOCAL_OBJS) $(CORE_LIB) $(OTHER_OBJS) -L. -lsupc++_nano -lm -lc_nano -lstdc++_nano

burn:
	make
	~/hardware/teensy-loader-cli/teensy_loader_cli -w -v -mmcu=mk20dx256 .build-teensy31/src.hex
