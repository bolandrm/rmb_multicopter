#include "motors.h"
// #include "flight_controller.h"

// M3 FTM0_CH0 - PTC1 - 22  FTM0_C0V PORTC_PCR1
// M4 FTM0_CH1 - PTC2 - 23  FTM0_C1V PORTC_PCR2
//    FTM0_CH2 - PTC3 - 9   FTM0_C2V PORTC_PCR3
//    FTM0_CH3 - PTC4 - 10  FTM0_C3V PORTC_PCR4
//    FTM0_CH4 - PTD4 - 6   FTM0_C4V PORTD_PCR4
// M1 FTM0_CH5 - PTD5 - 20  FTM0_C5V PORTD_PCR5
// M2 FTM0_CH6 - PTD6 - 21  FTM0_C6V PORTD_PCR6
//    FTM0_CH7 - PTD7 - 5   FTM0_C7V PORTD_PCR7

#define M1_OUTPUT_REG FTM0_C5V
#define M2_OUTPUT_REG FTM0_C6V
#define M3_OUTPUT_REG FTM0_C0V
#define M4_OUTPUT_REG FTM0_C1V

uint16_t outputs[NUM_MOTORS];
void _motors_command();

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
  // for(int i = 0; i < NUM_MOTORS; i++) {
  //   if (outputs[i] > INDOOR_SAFE_MOTOR_SPEED) {
  //     Serial.println("motors too high");
  //     fc_emergency_stop();
  //   }
  // }
}

void motors_set_output(int8_t motor_number, int16_t output) {
  outputs[motor_number] = output;
}

void motors_command() {
  adjust_for_bounds();
  motors_safety_check();
  _motors_command();
}

void _motors_command() {
  #ifdef ALLOW_MOTORS
    #ifdef M1_ON
      M1_OUTPUT_REG = outputs[M1] * 3;
    #else
      M1_OUTPUT_REG = 0;
    #endif

    #ifdef M2_ON
      M2_OUTPUT_REG = outputs[M2] * 3;
    #else
      M2_OUTPUT_REG = 0;
    #endif

    #ifdef M3_ON
      M3_OUTPUT_REG = outputs[M3] * 3;
    #else
      M3_OUTPUT_REG = 0;
    #endif

    #ifdef M4_ON
      M4_OUTPUT_REG = outputs[M4] * 3;
    #else
      M4_OUTPUT_REG = 0;
    #endif
  #else
    M1_OUTPUT_REG = 0;
    M2_OUTPUT_REG = 0;
    M3_OUTPUT_REG = 0;
    M4_OUTPUT_REG = 0;
  #endif
}

void motors_command_all_off() {
  zero_outputs();
  _motors_command(); // skip checks, just turn them off
}

void motors_init() {
  zero_outputs();

  // Flex timer0 configuration
  FTM0_SC = 0x0c;   // TOF=0 TOIE=0 CPWMS=0 CLKS=01 PS=100 (divide by 16)
  FTM0_MOD = 7500;  // 400Hz PWM signal

  // FTM0_C0SC = 0x28;   This appears to not be necessary

  motors_command_all_off();

  // PORT Configuration
  PORTD_PCR5 |= 0x400;
  PORTD_PCR6 |= 0x400;
  PORTC_PCR1 |= 0x400;
  PORTC_PCR2 |= 0x400;
}
