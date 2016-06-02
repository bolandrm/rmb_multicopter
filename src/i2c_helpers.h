#pragma once

#include <Arduino.h>

void i2c1_begin();
void i2c1_update_register(uint16_t device_address, uint8_t data_address, uint8_t data_value);
void i2c1_send_byte(uint16_t device_address, uint8_t data_value);
uint8_t i2c1_read_byte(uint16_t device_address);
uint16_t i2c1_read_word(uint16_t device_address);
