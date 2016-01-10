// [HEADER1][HEADER2][CODE][LENGTH_LSB][LENGTH_MSB][DATA][CRC]

#define PACKET_HEADER1 0x02
#define PACKET_HEADER2 0xB5

#define REQUEST_CONFIG 1
#define REQUEST_GYRO_ACC 2

#define SET_CONFIG 101

#include "serial_commands.h"
#include "config.h"
#include "pids.h"
#include "imu.h"
#include "flight_controller.h"

void read_serial_data(uint8_t data);
void process_serial_data();

static uint8_t state;
static uint8_t code;
static uint8_t incoming_crc;
static uint8_t outgoing_crc;
static uint16_t data_expected_length;
static uint16_t data_received_length;
static uint8_t data_buffer[200];

void serial_commands_process() {
  while (Serial.available()) {
    uint8_t data = Serial.read();
    read_serial_data(data);
  }
}

void read_serial_data(uint8_t data) {
  switch (state) {
    case 0:
      if (data == PACKET_HEADER1) {
        state++;
        Serial.println("got header1");
      }
      break;
    case 1:
      if (data == PACKET_HEADER2) {
        state++;
        Serial.println("got header2");
      } else {
        state = 0;
        Serial.println("header2 bad, resetting");
      }
      break;
    case 2:
      code = data;
      incoming_crc = data;
      Serial.print("code:");
      Serial.println(data);
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
      Serial.print("data length:");
      Serial.println(data_expected_length);
      state++;
      break;
    case 5:
      data_buffer[data_received_length] = data;
      incoming_crc ^= data;
      data_received_length++;

      if (data_received_length >= data_expected_length) {
        state++;
        Serial.print("got data length:");
        Serial.println(data_received_length);
      }
      break;
    case 6:
      Serial.print("calculated crc: ");
      Serial.println(incoming_crc);
      if (incoming_crc == data) {
        Serial.print("responding to code: ");
        Serial.println(code);
        // CRC is ok, process data
        process_serial_data();
      } else {
        Serial.println("crc bad");
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

void output_uint8(uint8_t value) {
  Serial.write(value);
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
  Serial.write(PACKET_HEADER1);
  Serial.write(PACKET_HEADER2);

  outgoing_crc = 0;

  output_uint8(code);
  output_uint16(size);
}

void protocol_tail() {
  Serial.write(outgoing_crc);
}

void process_serial_data() {
  switch (code) {
    case REQUEST_GYRO_ACC:
      packet_head(REQUEST_GYRO_ACC, 24);

      output_float32(imu_rates().x);
      output_float32(imu_rates().y);
      output_float32(imu_rates().z);
      output_float32(imu_angles().x);
      output_float32(imu_angles().y);
      output_float32(imu_angles().z);

      protocol_tail();

      //imu_gyro_angles();
      //imu_gyro_raws();
      //imu_accel_raws();
      //imu_accel_filtered();
      //imu_accel_angles();
      //packet_head(REQUEST_GYRO_ACC, 72);
      break;

    case SET_CONFIG:
      if (data_received_length == sizeof(CONFIG_union)) {

        CONFIG_union config;

        Serial.println("set config!");

        for (uint16_t i = 0; i < sizeof(CONFIG_union); i++) {
            config.raw[i] = data_buffer[i];
        }

        Serial.println(config.data.version);
        Serial.println(config.data.pid_rate_z.kd);
        Serial.println(config.data.pid_angle_z.i_max);

      } else {
        Serial.println("serial incorrect size");
        Serial.print("config size: ");
        Serial.print(sizeof(CONFIG_union));
        Serial.print(" data size: ");
        Serial.println(data_received_length);
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
