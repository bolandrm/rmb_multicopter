#include <i2c_t3.h>
#include "i2c_helpers.h"

void i2c_begin() {
  Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_INT, I2C_RATE_400);
}

void i2c_update_register(uint16_t device_address, uint8_t data_address, uint8_t data_value) {
  Wire.beginTransmission(device_address);
  Wire.write(data_address);
  Wire.write(data_value);
  Wire.endTransmission();
}

void i2c_send_byte(uint16_t device_address, uint8_t data_value) {
  Wire.beginTransmission(device_address);
  Wire.write(data_value);
  Wire.endTransmission();
}

uint8_t i2c_read_byte(uint16_t device_address) {
  Wire.requestFrom(device_address, 1);
  return Wire.read();
}

uint16_t i2c_read_word(uint16_t device_address) {
  Wire.requestFrom(device_address, 2);
  return (Wire.read() << 8) | Wire.read();
}
