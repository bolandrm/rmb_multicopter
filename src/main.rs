#![no_std]
#![feature(lang_items, alloc, collections)]

extern crate embedded_allocator;
extern crate alloc;
#[macro_use]
extern crate collections;

// use core::prelude::*;

pub mod runtime_support;
mod arduino;
mod c;

pub static SERIAL_PORT_SPEED:u32 = 115200;

#[no_mangle]
pub extern fn main() {
    c::setup();

    let mut vec = vec![1, 2, 3];

    vec.push(4);

    arduino::serial_begin(SERIAL_PORT_SPEED);

    arduino::digital_write(15, 0);

    loop {
        arduino::digital_write(14, 0);
        arduino::delay(1000);
        arduino::digital_write(14, 1);
        arduino::delay(1000);

        c::_yield();
    }
}

