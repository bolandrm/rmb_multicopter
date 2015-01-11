#ifndef I2C_HELPERS
#define I2C_HELPERS

#include <Wire.h>
#include <Arduino.h>

void i2c_update_register(int16_t device_address, uint8_t data_address, uint8_t data_value);
void i2c_send_byte(int16_t device_address, uint8_t data_value);
uint8_t i2c_read_byte(int16_t device_address);
uint16_t i2c_read_word(int16_t device_address);

#endif
