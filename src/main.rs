#![no_std]
#![feature(lang_items, alloc, collections)]

extern crate embedded_allocator;
extern crate alloc;

use core::prelude::*;

pub mod runtime_support;

#[macro_use]
extern crate collections;

use collections::string::*;
use collections::Vec;
use alloc::boxed::Box;

mod driver {
    extern {
        pub fn digitalWrite(pin: u8, val: u8);
        pub fn delay(duration: u16);
    }
}

#[no_mangle] pub extern fn run_loop() {
    let mut vec = vec![1, 2, 3];

    vec.push(4);

    loop {
        digital_write(14, 0);
        delay(200);
        digital_write(14, 1);
        delay(200);
    }
}

pub fn delay(ms: u16) {
    unsafe { driver::delay(ms) }
}

pub fn digital_write(pin: u8, val: u8) {
    unsafe { driver::digitalWrite(pin, val) }
}
