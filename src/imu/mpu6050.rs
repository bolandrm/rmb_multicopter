#![allow(dead_code)]

use arduino;
use debug;
use super::{AxisI16, AxisI32, AxisFloat};

const MPU6050_I2C_ADDRESS:u8        = 0x68;
const MPU6050_GYRO_500D_SENS:f32    = 65.5;
const MPU6050_GYRO_1000D_SENS:f32   = 32.8;
const MPU6050_ACCEL_4G_SENS:f32     = 8192.0;

// MPU 6000 registers
const MPUREG_WHOAMI:u8         = 0x75;
const MPUREG_SMPLRT_DIV:u8     = 0x19;
const MPUREG_CONFIG:u8         = 0x1A;
const MPUREG_GYRO_CONFIG:u8    = 0x1B;
const MPUREG_ACCEL_CONFIG:u8   = 0x1C;
const MPUREG_FIFO_EN:u8        = 0x23;
const MPUREG_INT_PIN_CFG:u8    = 0x37;
const MPUREG_INT_ENABLE:u8     = 0x38;
const MPUREG_INT_STATUS:u8     = 0x3A;
const MPUREG_ACCEL_XOUT_H:u8   = 0x3B;
const MPUREG_ACCEL_XOUT_L:u8   = 0x3C;
const MPUREG_ACCEL_YOUT_H:u8   = 0x3D;
const MPUREG_ACCEL_YOUT_L:u8   = 0x3E;
const MPUREG_ACCEL_ZOUT_H:u8   = 0x3F;
const MPUREG_ACCEL_ZOUT_L:u8   = 0x40;
const MPUREG_TEMP_OUT_H:u8     = 0x41;
const MPUREG_TEMP_OUT_L:u8     = 0x42;
const MPUREG_GYRO_XOUT_H:u8    = 0x43;
const MPUREG_GYRO_XOUT_L:u8    = 0x44;
const MPUREG_GYRO_YOUT_H:u8    = 0x45;
const MPUREG_GYRO_YOUT_L:u8    = 0x46;
const MPUREG_GYRO_ZOUT_H:u8    = 0x47;
const MPUREG_GYRO_ZOUT_L:u8    = 0x48;
const MPUREG_USER_CTRL:u8      = 0x6A;
const MPUREG_PWR_MGMT_1:u8     = 0x6B;
const MPUREG_PWR_MGMT_2:u8     = 0x6C;
const MPUREG_FIFO_COUNTH:u8    = 0x72;
const MPUREG_FIFO_COUNTL:u8    = 0x73;
const MPUREG_FIFO_R_W:u8       = 0x74;

// Configuration bits
const BIT_SLEEP:u8                   = 0x40;
const BIT_H_RESET:u8                 = 0x80;
const BITS_CLKSEL:u8                 = 0x07;
const MPU_CLK_SEL_PLLGYROX:u8        = 0x01;
const MPU_CLK_SEL_PLLGYROZ:u8        = 0x03;
const MPU_EXT_SYNC_GYROX:u8          = 0x02;
const BITS_FS_250DPS:u8              = 0x00;
const BITS_FS_500DPS:u8              = 0x08;
const BITS_FS_1000DPS:u8             = 0x10;
const BITS_FS_2000DPS:u8             = 0x18;
const BITS_FS_MASK:u8                = 0x18;
const BITS_DLPF_CFG_256HZ_NOLPF2:u8  = 0x00;
const BITS_DLPF_CFG_188HZ:u8         = 0x01;
const BITS_DLPF_CFG_98HZ:u8          = 0x02;
const BITS_DLPF_CFG_42HZ:u8          = 0x03;
const BITS_DLPF_CFG_20HZ:u8          = 0x04;
const BITS_DLPF_CFG_10HZ:u8          = 0x05;
const BITS_DLPF_CFG_5HZ:u8           = 0x06;
const BITS_DLPF_CFG_2100HZ_NOLPF:u8  = 0x07;
const BITS_DLPF_CFG_MASK:u8          = 0x07;
const BIT_INT_ANYRD_2CLEAR:u8        = 0x10;
const BIT_RAW_RDY_EN:u8              = 0x01;
const BIT_I2C_IF_DIS:u8              = 0x10;
const BIT_INT_STATUS_DATA:u8         = 0x01;

mod helpers {
    use i2c;
    use arduino;

    pub fn write_reg(addr: u8, data: u8) {
        i2c::update_register(super::MPU6050_I2C_ADDRESS, addr, data);
        arduino::delay(1);
    }

    pub fn read_byte(addr: u8) -> u8 {
        i2c::send_byte(super::MPU6050_I2C_ADDRESS, addr);
        return i2c::read_byte(super::MPU6050_I2C_ADDRESS);
    }

    pub fn read_word(addr: u8) -> u16 {
        i2c::send_byte(super::MPU6050_I2C_ADDRESS, addr);
        return i2c::read_word(super::MPU6050_I2C_ADDRESS);
    }

    pub fn test_connection() -> bool {
        return read_byte(super::MPUREG_WHOAMI) == 0x68;
    }
}

pub struct MPU6050;

impl MPU6050 {
    pub fn init() {
        helpers::write_reg(MPUREG_PWR_MGMT_1, BIT_H_RESET);
        arduino::delay(100);  // Startup time delay

        // Wake Up device and select GyroZ clock (better performance)
        helpers::write_reg(MPUREG_PWR_MGMT_1, MPU_CLK_SEL_PLLGYROZ);
        helpers::write_reg(MPUREG_PWR_MGMT_2, 0);

        // SAMPLE RATE
        helpers::write_reg(MPUREG_SMPLRT_DIV, 0x00);     // Sample rate = 1kHz

        // FS & DLPF   FS=1000º/s, DLPF = 42Hz (low pass filter)
        helpers::write_reg(MPUREG_CONFIG, BITS_DLPF_CFG_42HZ);
        helpers::write_reg(MPUREG_GYRO_CONFIG, BITS_FS_1000DPS);  // Gyro scale 1000º/s
        helpers::write_reg(MPUREG_ACCEL_CONFIG, 0x08);   // Accel scale +-4g

        arduino::delay(100);  // Startup time delay

        if helpers::test_connection() == true {
            serial_println!("Connected to MPU6050!");
        } else {
            loop {
                serial_println!("Unable to connect to MPU6050.");
                arduino::delay(1000);
            }
        }
    }

    pub fn read_gyro(gyro_rates: &mut AxisI16) {
        let gyro_x: i16 = helpers::read_word(MPUREG_GYRO_XOUT_H) as i16;
        let gyro_y: i16 = helpers::read_word(MPUREG_GYRO_YOUT_H) as i16;
        let gyro_z: i16 = helpers::read_word(MPUREG_GYRO_ZOUT_H) as i16;

        gyro_rates.x = gyro_x;
        gyro_rates.y = gyro_y;
        gyro_rates.z = gyro_z;
    }
}
