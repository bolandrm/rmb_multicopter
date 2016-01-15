#![no_std]
#![feature(lang_items, alloc, collections)]
// #![feature(clone_from_slice)]

extern crate embedded_allocator;
extern crate alloc;
#[macro_use]
extern crate collections;

//use collections::string::*;
// use core::prelude::*;

pub mod runtime_support;
mod arduino;
mod c;

#[no_mangle]
pub extern fn main() {
    c::setup();

    arduino::pin_mode(14, arduino::OUTPUT);
    arduino::pin_mode(15, arduino::OUTPUT);

    let mut vec = vec![1, 2, 3];
    vec.push(4);

    arduino::digital_write(15, 0);


    loop {
        arduino::digital_write(14, arduino::LOW);
        arduino::digital_write(15, arduino::HIGH);
        arduino::delay(300);
        arduino::digital_write(14, arduino::HIGH);
        arduino::digital_write(15, arduino::LOW);
        arduino::delay(300);

        arduino::usb_serial_println("ok then 2");

        c::_loop();

        c::_yield();
    }
}

