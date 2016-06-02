#include <i2c_t3.h>
#include "i2c_helpers.h"

void i2c1_begin() {
  Wire1.begin(I2C_MASTER, 0x00, I2C_PINS_29_30, I2C_PULLUP_EXT, I2C_RATE_800);
}

void i2c1_update_register(uint16_t device_address, uint8_t data_address, uint8_t data_value) {
  Wire1.beginTransmission(device_address);
  Wire1.write(data_address);
  Wire1.write(data_value);
  Wire1.endTransmission();
}

void i2c1_send_byte(uint16_t device_address, uint8_t data_value) {
  Wire1.beginTransmission(device_address);
  Wire1.write(data_value);
  Wire1.endTransmission();
}

uint8_t i2c1_read_byte(uint16_t device_address) {
  Wire1.requestFrom(device_address, 1);
  return Wire1.read();
}

uint16_t i2c1_read_word(uint16_t device_address) {
  Wire1.requestFrom(device_address, 2);
  return (Wire1.read() << 8) | Wire1.read();
}
