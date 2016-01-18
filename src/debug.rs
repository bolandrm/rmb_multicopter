use arduino;
use collections::string::*;
extern crate collections;

pub fn serial_print(string: collections::string::String) {
    let _str = string.as_str();
    arduino::usb_serial_write(_str);
}

pub fn serial_println(string: collections::string::String) {
    serial_print(string);
    serial_print("\r\n".to_string());
}

macro_rules! serial_print(
    ( $($args:expr),+ ) => (
        debug::serial_print(format!( $( $args ),+ ));
    );
);

macro_rules! serial_println(
    ( $($args:expr),+ ) => (
        debug::serial_println(format!( $( $args ),+ ));
    );
);
