#![allow(dead_code)]

pub const HIGH:u8           = 1;
pub const LOW:u8            = 0;
pub const INPUT:u8          = 0;
pub const OUTPUT:u8         = 1;
pub const INPUT_PULLUP:u8   = 2;
pub const CHANGE:u8         = 4;
pub const FALLING:u8        = 2;
pub const RISING:u8         = 3;

pub fn digital_write(pin: u8, val: u8) {
    unsafe { native::digitalWrite(pin, val); }
}

pub fn pin_mode(pin: u8, mode: u8) {
    unsafe { native::pinMode(pin, mode); }
}

pub fn delay(ms: u16) {
    unsafe { native::delay(ms); }
}

pub fn usb_serial_write(str: &str) {
    unsafe { native::usb_serial_write(str.as_bytes().as_ptr(), str.len()); }
}

mod native {
    use c::types::*;

    extern {
        pub fn pinMode(pin: u8, mode: u8);
        pub fn digitalWrite(pin: u8, val: u8);

        pub fn delay(duration: u16);

        pub fn usb_serial_write(buffer: *const c_char, size: c_size_t);
    }
}

