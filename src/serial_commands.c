// [HEADER1][HEADER2][CODE][LENGTH_LSB][LENGTH_MSB][DATA][CRC]

#define PACKET_HEADER1 0x02
#define PACKET_HEADER2 0xB5

#define REQUEST_CONFIG 1
#define REQUEST_GYRO_ACC 2
#define REQUEST_RC 3
#define REQUEST_MOTORS 4

#define SET_CONFIG 101

#define INFO_SUCCESS 201
#define INFO_FAILURE 202
#define INFO_BAD_CRC 203

#include "serial_commands.h"
#include "config.h"
#include "pids.h"
#include "imu.h"
#include "flight_controller.h"
#include "motors.h"
#include "utils.h"
#include <usb_serial.h>

void read_serial_data(uint8_t data);
void process_serial_data();
void send_bad_crc();
CONFIG_union current_config();

static uint8_t state;
static uint8_t code;
static uint8_t incoming_crc;
static uint8_t outgoing_crc;
static uint16_t data_expected_length;
static uint16_t data_received_length;
static uint8_t data_buffer[200];

void serial_commands_process() {
  while (usb_serial_available()) {
    uint8_t data = usb_serial_getchar();
    read_serial_data(data);
  }
}

void read_serial_data(uint8_t data) {
  switch (state) {
    case 0:
      if (data == PACKET_HEADER1) {
        state++;
      }
      break;
    case 1:
      if (data == PACKET_HEADER2) {
        state++;
      } else {
        state = 0;
      }
      break;
    case 2:
      code = data;
      incoming_crc = data;
      state++;
      break;
    case 3:  // Data length LSB
      data_expected_length = data;
      incoming_crc ^= data;
      state++;
      break;
    case 4:  // Data length MSB
      data_expected_length |= (data << 8);
      incoming_crc ^= data;
      state++;
      break;
    case 5:
      data_buffer[data_received_length] = data;
      incoming_crc ^= data;
      data_received_length++;

      if (data_received_length >= data_expected_length) {
        state++;
      }
      break;
    case 6:
      if (incoming_crc == data) {
        process_serial_data();
      } else {
        send_bad_crc();
      }

      // reset variables
      memset(data_buffer, 0, sizeof(data_buffer));
      data_received_length = 0;
      state = 0;
      break;
  }
}

void output_uint8(uint8_t value) {
  usb_serial_putchar(value);
  outgoing_crc ^= value;
}

void output_uint16(uint16_t value) {
  output_uint8(lowByte(value));
  output_uint8(highByte(value));
}

void output_uint32(uint32_t value) {
  for (uint8_t i = 0; i < 4; i++) {
    output_uint8((uint8_t) (value >> (i * 8)));
  }
}

void output_float32(float value) {
  uint8_t *bytes = (uint8_t *) &value;

  for (uint8_t i = 0; i < sizeof(value); i++) {
    output_uint8(bytes[i]);
  }
}

void packet_head(uint8_t code, uint16_t size) {
  usb_serial_putchar(PACKET_HEADER1);
  usb_serial_putchar(PACKET_HEADER2);

  outgoing_crc = 0;

  output_uint8(code);
  output_uint16(size);
}

void packet_tail() {
  usb_serial_putchar(outgoing_crc);
}

void send_code_without_data(uint8_t code) {
  packet_head(code, 0);
  output_uint8(0x00);
  packet_tail();
}

void send_success() {
  send_code_without_data(INFO_SUCCESS);
}

void send_failure() {
  send_code_without_data(INFO_FAILURE);
}

void send_bad_crc() {
  send_code_without_data(INFO_BAD_CRC);
}

void process_serial_data() {
  switch (code) {
    case REQUEST_CONFIG:
      packet_head(REQUEST_CONFIG, sizeof(CONFIG));

      for (uint16_t i = 0; i < sizeof(CONFIG); i++) {
        output_uint8(CONFIG.raw[i]);
      }

      packet_tail();
      break;

    case REQUEST_GYRO_ACC:
      packet_head(REQUEST_GYRO_ACC, 92);

      output_float32(imu_rates().x);
      output_float32(imu_rates().y);
      output_float32(imu_rates().z);

      output_float32(imu_angles().x);
      output_float32(imu_angles().y);
      output_float32(imu_angles().z);

      output_float32(imu_gyro_angles().x);
      output_float32(imu_gyro_angles().y);
      output_float32(imu_gyro_angles().z);

      output_uint32(imu_gyro_raws().x);
      output_uint32(imu_gyro_raws().y);
      output_uint32(imu_gyro_raws().z);

      output_uint32(imu_accel_raws().x);
      output_uint32(imu_accel_raws().y);
      output_uint32(imu_accel_raws().z);

      output_float32(imu_accel_filtered().x);
      output_float32(imu_accel_filtered().y);
      output_float32(imu_accel_filtered().z);

      output_float32(imu_accel_angles().x);
      output_float32(imu_accel_angles().y);
      output_float32(imu_accel_angles().z);

      output_float32(imu_gyro_max_value());
      output_float32(imu_accel_max_value());

      packet_tail();
      break;

    case REQUEST_RC:
      packet_head(REQUEST_RC, 15 * RC_NUM_CHANNELS);

      for (uint8_t i = 0; i < RC_NUM_CHANNELS; i++) {
        uint8_t function = CONFIG.data.rc_channels[i].function;

        output_uint8(function);
        output_uint16(rc_get_raw(function));
        output_float32(rc_get(function));
        output_float32(rc_out_min(function));
        output_float32(rc_out_max(function));
      }

      packet_tail();
      break;

    case REQUEST_MOTORS:
      packet_head(REQUEST_MOTORS, 8);

      for (uint8_t i = 0; i < NUM_MOTORS; i++) {
        output_uint16(motor_level(i));
      }

      packet_tail();
      break;

    case SET_CONFIG:
      if (data_received_length == sizeof(CONFIG)) {

        uint16_t received_config_version = (data_buffer[0] | (data_buffer[1] << 8));

        if (received_config_version != CONFIG_VERSION) {
          send_failure();
          break;
        }

        config_set(data_buffer);

        send_success();

      } else {
        send_failure();
      }
      break;
  }
}
