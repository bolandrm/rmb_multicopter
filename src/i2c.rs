#![allow(dead_code)]

mod native {
    extern {
        pub fn i2c_begin();
        pub fn i2c_update_register(device_address: u8, data_address: u8, data_value: u8);
        pub fn i2c_send_byte(device_address: u8, data_value: u8);
        pub fn i2c_read_byte(device_address: u8) -> u8;
        pub fn i2c_read_word(device_address: u8) -> u16;
    }
}

pub fn begin() {
    unsafe { native::i2c_begin(); }
}

pub fn update_register(device_address: u8, data_address: u8, data_value: u8) {
    unsafe { native::i2c_update_register(device_address, data_address, data_value); }
}

pub fn send_byte(device_address: u8, data_value: u8) {
    unsafe { native::i2c_send_byte(device_address, data_value); }
}

pub fn read_byte(device_address: u8) -> u8 {
    unsafe { return native::i2c_read_byte(device_address); }
}

pub fn read_word(device_address: u8) -> u16 {
    unsafe { return native::i2c_read_word(device_address); }
}
