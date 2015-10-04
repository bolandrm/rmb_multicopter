// [HEADER1][HEADER2][CODE][LENGTH_LSB][LENGTH_MSB][DATA][CRC]

#define PACKET_HEADER1 0x02
#define PACKET_HEADER2 0xB5

#define REQUEST_CONFIG 1
#define REQUEST_GYRO_ACC 1

#include "serial_commands.h"
#include "pids.h"
#include "flight_controller.h"

void read_serial_data(uint8_t data);

uint8_t data;
uint8_t state;
uint8_t code;
uint8_t incoming_crc;
uint8_t outgoing_crc;
uint16_t data_expected_length;
uint16_t data_received_length;
uint8_t data_buffer[200];

void serial_commands_process() {
  while (Serial.available()) {
    data = Serial.read();
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
        // CRC is ok, process data
        // process_data();
      } else {
        // respond that CRC failed
        // CRC_FAILED(code, incoming_crc);
      }

      // reset variables
      memset(data_buffer, 0, sizeof(data_buffer));
      data_received_length = 0;
      state = 0;
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
