#![no_std]
#![feature(lang_items, alloc, collections, convert)]

extern crate embedded_allocator;
extern crate alloc;
#[macro_use] extern crate collections;

#[macro_use] mod debug;
pub mod runtime_support;
mod arduino;
mod c;
mod i2c;
mod mpu;

#[no_mangle]
pub extern fn main() {
    mpu::MPU6050::init();

    c::setup();

    arduino::pin_mode(14, arduino::OUTPUT);
    arduino::pin_mode(15, arduino::OUTPUT);

    let mut vec = vec![1, 2, 3];
    vec.push(4);

    arduino::digital_write(15, 0);

    let mut gyro_rates = mpu::AxisI16 { x: 0, y: 0, z: 0 };

    loop {
        arduino::digital_write(14, arduino::LOW);
        arduino::digital_write(15, arduino::HIGH);
        arduino::delay(300);
        arduino::digital_write(14, arduino::HIGH);
        arduino::digital_write(15, arduino::LOW);
        arduino::delay(300);

        mpu::MPU6050::read_gyro(&mut gyro_rates);
        serial_println!("x: {}, y: {}, z: {}", gyro_rates.x, gyro_rates.y, gyro_rates.z);

        c::_loop();

        c::_yield();
    }
}

