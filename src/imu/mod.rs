pub mod mpu6050;
use i2c;

pub struct AxisI16 { pub x: i16, pub y: i16, pub z: i16 }
pub struct AxisI32 { pub x: i32, pub y: i32, pub z: i32 }
pub struct AxisFloat { pub x: f32, pub y: f32, pub z: f32 }

pub struct IMU {
    pub imu_gyro_raws: AxisI16
}

impl IMU {
    pub fn new() -> IMU {
        i2c::begin();
        mpu6050::MPU6050::init();

        IMU {
            imu_gyro_raws: AxisI16 { x: 0, y: 0, z: 0 }
        }
    }

    pub fn read_raw_values(&mut self) {
        mpu6050::MPU6050::read_gyro(&mut self.imu_gyro_raws);
    }
}
