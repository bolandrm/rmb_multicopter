#ifndef I2C_HELPERS_H
#define I2C_HELPERS_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif
  void i2c_begin();
  void i2c_update_register(uint16_t device_address, uint8_t data_address, uint8_t data_value);
  void i2c_send_byte(uint16_t device_address, uint8_t data_value);
  uint8_t i2c_read_byte(uint16_t device_address);
  uint16_t i2c_read_word(uint16_t device_address);
#ifdef __cplusplus
}
#endif

#endif
