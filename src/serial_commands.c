// [HEADER1][HEADER2][CODE][LENGTH_LSB][LENGTH_MSB][DATA][CRC]

#define PACKET_HEADER1 0x02
#define PACKET_HEADER2 0xB5

#define REQUEST_CONFIG 1
#define REQUEST_GYRO_ACC 2

#define SET_CONFIG 101

#define INFO_SUCCESS 201
#define INFO_FAILURE 202
#define INFO_BAD_CRC 203

#include "serial_commands.h"
#include "config.h"
#include "pids.h"
#include "imu.h"
#include "flight_controller.h"
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
  packet_head(INFO_SUCCESS, 0);
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
      packet_head(REQUEST_GYRO_ACC, 24);

      output_float32(imu_rates().x);
      output_float32(imu_rates().y);
      output_float32(imu_rates().z);
      output_float32(imu_angles().x);
      output_float32(imu_angles().y);
      output_float32(imu_angles().z);

      packet_tail();

      //imu_gyro_angles();
      //imu_gyro_raws();
      //imu_accel_raws();
      //imu_accel_filtered();
      //imu_accel_angles();
      //packet_head(REQUEST_GYRO_ACC, 72);
      break;

    case SET_CONFIG:
      if (data_received_length == sizeof(CONFIG)) {

        //if ((uint16_t)data_buffer[0] != CONFIG_VERSION) {
        //  send_failure();
        //  break;
        //}

        for (uint16_t i = 0; i < sizeof(CONFIG); i++) {
            CONFIG.raw[i] = data_buffer[i];
        }

        send_success();

      } else {
        send_failure();
      }
      break;
  }
}

void serial_update_pids(byte incomingByte) {
  double kp, ki, kd;

  if (fc_mode() == STABILIZE) {
    kp = pid(PID_ANGLE_X)->kp;
    ki = pid(PID_ANGLE_X)->ki;
    kd = pid(PID_ANGLE_X)->kd;
  } else {
    kp = pid(PID_RATE_X)->kp;
    ki = pid(PID_RATE_X)->ki;
    kd = pid(PID_RATE_X)->kd;
  }

  if (incomingByte == 'a') {
    if (kp <= 0.05) kp = 0;
    else kp -= 0.05;
  } else if (incomingByte == 's') {
    if (ki <= 0.05) ki = 0;
    else ki -= 0.05;
  } else if (incomingByte == 'd') {
     if (kd <= 0.05) kd = 0;
     else kd -= 0.05;
  } else if (incomingByte == 'q') {
    if (kp == 0) kp = 0.01;
    else kp += 0.05;
  } else if (incomingByte == 'w') {
    if (ki == 0) ki = 0.01;
    else ki += 0.05;
  } else if (incomingByte == 'e') {
     if (kd == 0) kd = 0.01;
     else kd += 0.05;
  }

  if (fc_mode() == STABILIZE) {
    pid(PID_ANGLE_X)->kp = kp;
    pid(PID_ANGLE_X)->ki = ki;
    pid(PID_ANGLE_X)->kd = kd;

    pid(PID_ANGLE_Y)->kp = kp;
    pid(PID_ANGLE_Y)->ki = ki;
    pid(PID_ANGLE_Y)->kd = kd;
  } else {
    pid(PID_RATE_X)->kp = kp;
    pid(PID_RATE_X)->ki = ki;
    pid(PID_RATE_X)->kd = kd;

    pid(PID_RATE_Y)->kp = kp;
    pid(PID_RATE_Y)->ki = ki;
    pid(PID_RATE_Y)->kd = kd;
  }
}
