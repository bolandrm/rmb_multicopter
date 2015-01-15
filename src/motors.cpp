#include "motors.h"
#include "flight_controller.h"

#define M1_PIN 3
#define M2_PIN 9
#define M3_PIN 10
#define M4_PIN 11

#define M1_OUTPUT_REG OCR2B
#define M2_OUTPUT_REG OCR1A
#define M3_OUTPUT_REG OCR1B
#define M4_OUTPUT_REG OCR2A

uint16_t outputs[NUM_MOTORS];

void zero_outputs() {
  for (int i = 0; i < NUM_MOTORS; i++) {
    outputs[i] = MOTOR_SAFE_OFF;
  }
}

int16_t motor_level(int8_t motor_number) {
  return outputs[motor_number];
}

void adjust_for_bounds() {
  int16_t motor_fix = 0;
  uint16_t motor_min = outputs[0];
  uint16_t motor_max = outputs[0];

  for(int i = 1; i < NUM_MOTORS; i++) {
    if (outputs[i] < motor_min) motor_min = outputs[i];
    if (outputs[i] > motor_max) motor_max = outputs[i];
  }

  if (motor_min < MOTOR_MIN) {
    motor_fix = MOTOR_MIN - motor_min;
  } else if (motor_max > MOTOR_MAX) {
    motor_fix = MOTOR_MAX - motor_max;
  }

  for(int i = 0; i < NUM_MOTORS; i++) {
    outputs[i] += motor_fix;
  }
}

void motors_safety_check() {
  for(int i = 0; i < NUM_MOTORS; i++) {
    if (outputs[i] > INDOOR_SAFE_MOTOR_SPEED) {
      Serial.println("motors too high");
      fc_emergency_stop();
    }
  }
}

void motors_set_output(int8_t motor_number, int16_t output) {
  outputs[motor_number] = output;
}

void motors_command() {
  adjust_for_bounds();
  motors_safety_check();

  #ifdef ALLOW_MOTORS
    M1_OUTPUT_REG = outputs[M1] / 16;
    M2_OUTPUT_REG = outputs[M2] * 2;
    M3_OUTPUT_REG = outputs[M3] * 2;
    M4_OUTPUT_REG = outputs[M4] / 16;
  #else
    M1_OUTPUT_REG = 0;
    M2_OUTPUT_REG = 0;
    M3_OUTPUT_REG = 0;
    M4_OUTPUT_REG = 0;
  #endif
}

void motors_command_all_off() {
  zero_outputs();
  motors_command();
}

void motors_init() {
  zero_outputs();

  pinMode(M1_PIN, OUTPUT);
  pinMode(M2_PIN, OUTPUT);
  pinMode(M3_PIN, OUTPUT);
  pinMode(M4_PIN, OUTPUT);

  // Init PWM Timer 1  16 bit
  // // Clear OCnA/OCnB/OCnC on compare match, set OCnA/OCnB/OCnC at BOTTOM (non-inverting mode)
  // // Prescaler set to 8, that gives us a resolution of 0.5us
  TCCR1A = (1<<WGM11)|(1<<COM1A1)|(1<<COM1B1);
  TCCR1B = (1<<WGM13)|(1<<WGM12)|(1<<CS11);
  ICR1 = PWM_COUNTER_PERIOD;

  // Init PWM Timer 2   8bit                                 
  // WGMn1 WGMn2 = Mode ? Fast PWM, TOP = 0xFF ,Update of OCRnx at BOTTOM
  // Clear OCnA/OCnB on compare match, set OCnA/OCnB at BOTTOM (non-inverting mode)
  // Prescaler set to 256, that gives us a resolution of 16us
  // Output_PWM_Frequency = 244hz = 16000000/(256*(1+255)) = Clock_Speed / (Prescaler * (1 + TOP))
  // TOP is fixed at 255                                     
  TCCR2A = (1<<WGM20)|(1<<WGM21)|(1<<COM2A1)|(1<<COM2B1);
  TCCR2B = (1<<CS22)|(1<<CS21);
}
