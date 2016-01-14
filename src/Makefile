ARDUINO_DIR   = /Applications/Arduino.app/Contents/Resources/Java
USER_LIB_PATH = ./libraries
OBJDIR        = ./.build-$(BOARD_TAG)
AVR_TOOLS_PATH = /Users/bolandrm/hardware/gcc-arm-none-eabi-4_9-2014q4/bin

BOARD_TAG    = teensy31
ARDUINO_LIBS = MedianFilter i2c_t3

NO_CORE_MAIN_CPP = true

MONITOR_BAUDRATE  = 115200
MONITOR_PORT      = /dev/tty.usbmodem*

LOCAL_CPP_SRCS = $(wildcard *.cpp) $(wildcard ./hardware/*.cpp)
CPPFLAGS = -I./hardware -I.

include ~/hardware/Arduino-Makefile/Teensy.mk

LDFLAGS =  $(call PARSE_TEENSY,$(BOARD_TAG),build.linkoption) $(call PARSE_TEENSY,$(BOARD_TAG),build.additionalobject)
LDFLAGS += -T$(call PARSE_TEENSY,$(BOARD_TAG),build.linkscript)
LDFLAGS += -$(MCU_FLAG_NAME)=$(MCU) -Wl,--gc-sections -O$(OPTIMIZATION_LEVEL)
LDFLAGS2 = $(filter-out -larm_cortexM4l_math,$(LDFLAGS))

RUST_OBJS = $(wildcard $(RUST_LIBS_DIR)/*.o) $(OBJDIR)/rust.o

$(TARGET_ELF): 	$(LOCAL_OBJS) $(CORE_LIB) $(OTHER_OBJS) $(RUST_OBJS)
		$(CC) $(LDFLAGS2) -o $@ $(LOCAL_OBJS) $(CORE_LIB) $(OTHER_OBJS) $(RUST_OBJS) -L. -lm -lc_nano 

TEENSY_LOADER_CLI = ~/hardware/teensy_loader_cli/teensy_loader_cli
REBOOT_TEENSY = stty -f $(MONITOR_PORT) 134

burn:
	make
	if [ -a $(MONITOR_PORT) ]; then $(REBOOT_TEENSY); fi;
	$(TEENSY_LOADER_CLI) -w -v -mmcu=mk20dx256 .build-teensy31/src.hex

reboot:
	$(REBOOT_TEENSY)

boot:
	$(TEENSY_LOADER_CLI) -b -v -mmcu=mk20dx256

RUST_DIR = /Users/bolandrm/hardware/rust
RUST_TARGET_DIR = ./.build-rust-arm
RUST_LIBS_DIR = $(RUST_TARGET_DIR)/rust_libs

rust_libs:
	@$(MKDIR) $(RUST_LIBS_DIR)
	rustc -C ar="$(AR)" -C opt-level=2 -Z no-landing-pads --target thumbv7em-none-eabi -g $(RUST_DIR)/src/libcore/lib.rs --out-dir $(RUST_LIBS_DIR) --emit link,obj
	rustc -C ar="$(AR)" --cfg feature=\"external_funcs\" -C opt-level=2 -Z no-landing-pads --target thumbv7em-none-eabi -g $(RUST_DIR)/src/liballoc/lib.rs --out-dir $(RUST_LIBS_DIR) -L $(RUST_LIBS_DIR) --emit link,obj
	rustc -C ar="$(AR)" -C opt-level=2 -Z no-landing-pads --target thumbv7em-none-eabi -g $(RUST_DIR)/src/librustc_unicode/lib.rs --out-dir $(RUST_LIBS_DIR) -L $(RUST_LIBS_DIR) --emit link,obj
	rustc -C ar="$(AR)" -C opt-level=2 -Z no-landing-pads --target thumbv7em-none-eabi -g $(RUST_DIR)/src/libcollections/lib.rs --out-dir $(RUST_LIBS_DIR) -L $(RUST_LIBS_DIR) --emit link,obj
	rustc -C ar="$(AR)" -C opt-level=2 -Z no-landing-pads --target thumbv7em-none-eabi -g embedded_allocator/lib.rs --out-dir $(RUST_LIBS_DIR) -L $(RUST_LIBS_DIR) --emit link,obj

$(OBJDIR)/rust.o: $(wildcard *.rs)
	rustc -C ar="$(AR)" -C opt-level=2 -Z no-landing-pads --target thumbv7em-none-eabi -g --crate-type staticlib -L $(RUST_LIBS_DIR) main.rs -o $@ --emit obj
