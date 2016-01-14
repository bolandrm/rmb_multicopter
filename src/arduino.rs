mod native {
    extern {
        pub fn digitalWrite(pin: u8, val: u8);

        pub fn delay(duration: u16);

        pub fn serial_begin(divisor: u32);
    }
}

pub fn digital_write(pin: u8, val: u8) {
    unsafe { native::digitalWrite(pin, val) }
}

pub fn delay(ms: u16) {
    unsafe { native::delay(ms) }
}

pub fn serial_begin(divisor: u32) {
    unsafe { native::serial_begin(divisor) }
}
