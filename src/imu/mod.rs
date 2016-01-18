pub mod mpu6050;
#[macro_use] use debug;
use self::mpu6050::MPU6050;

pub struct AxisI16 { pub x: i16, pub y: i16, pub z: i16 }
pub struct AxisI32 { pub x: i32, pub y: i32, pub z: i32 }
pub struct AxisFloat { pub x: f32, pub y: f32, pub z: f32 }

pub struct IMU {
    pub gyro_raws: AxisI16,
    pub gyro_sums: AxisFloat,
    gyro_sum_count: i16,

    pub accel_raws: AxisI16,

    mpu: MPU6050,
}

impl IMU {
    pub fn new() -> IMU {
        IMU {
            mpu: MPU6050::new(),

            gyro_raws: AxisI16 { x: 0, y: 0, z: 0 },
            gyro_sums: AxisFloat { x: 0.0, y: 0.0, z: 0.0 },
            gyro_sum_count: 0,

            accel_raws: AxisI16 { x: 0, y: 0, z: 0 },
        }
    }

    pub fn read_raw_values(&mut self) {
        self.read_gyro_raws();
    }

    fn read_gyro_raws(&mut self) {
        self.mpu.read_gyro(&mut self.gyro_raws);

        // self.gyro_sums.y += self.gyro_raws.y as f32;
        // self.gyro_sums.z += self.gyro_raws.z as f32;

        // self.gyro_sum_count += 1;
    }

    fn reset_gyro_sums(&mut self) {
        self.gyro_sums.x = 0.0;
        self.gyro_sums.y = 0.0;
        self.gyro_sums.z = 0.0;
        self.gyro_sum_count = 0;
    }
}
