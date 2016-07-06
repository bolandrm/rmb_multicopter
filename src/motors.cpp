// Motors controlled with oneshot125 (ESCs are running SimonK firmware)
// https://github.com/sim-/tgy/commit/6dbc056a71161593cc8eaf08f8959cf4ea6e4ce6

#include "motors.h"
#include "flight_controller.h"
#include "utils.h"

// M3 FTM0_CH0 - PTC1 - 22  FTM0_C0V PORTC_PCR1
// M4 FTM0_CH1 - PTC2 - 23  FTM0_C1V PORTC_PCR2
//    FTM0_CH2 - PTC3 - 9   FTM0_C2V PORTC_PCR3
//    FTM0_CH3 - PTC4 - 10  FTM0_C3V PORTC_PCR4
//    FTM0_CH4 - PTD4 - 6   FTM0_C4V PORTD_PCR4
// M1 FTM0_CH5 - PTD5 - 20  FTM0_C5V PORTD_PCR5
// M2 FTM0_CH6 - PTD6 - 21  FTM0_C6V PORTD_PCR6
//    FTM0_CH7 - PTD7 - 5   FTM0_C7V PORTD_PCR7

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
  for(int i = 0; i < NUM_MOTORS; i++) {
    if (outputs[i] > INDOOR_SAFE_MOTOR_SPEED) {
      fc_emergency_stop("motors too high");
    }
  }
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
  FTM0_SC = 0x0; //disable clock
  FTM0_CNT = 0x0; // reset count

  #ifdef ALLOW_MOTORS
    #ifdef M1_ON
      M1_OUTPUT_REG = utils_round_positive(outputs[M1] * MOTOR_VALUE_SCALE_FACTOR);
    #else
      M1_OUTPUT_REG = 0;
    #endif

    #ifdef M2_ON
      M2_OUTPUT_REG = utils_round_positive(outputs[M2] * MOTOR_VALUE_SCALE_FACTOR);
    #else
      M2_OUTPUT_REG = 0;
    #endif

    #ifdef M3_ON
      M3_OUTPUT_REG = utils_round_positive(outputs[M3] * MOTOR_VALUE_SCALE_FACTOR);
    #else
      M3_OUTPUT_REG = 0;
    #endif

    #ifdef M4_ON
      M4_OUTPUT_REG = utils_round_positive(outputs[M4] * MOTOR_VALUE_SCALE_FACTOR);
    #else
      M4_OUTPUT_REG = 0;
    #endif
  #else
    M1_OUTPUT_REG = 0;
    M2_OUTPUT_REG = 0;
    M3_OUTPUT_REG = 0;
    M4_OUTPUT_REG = 0;
  #endif

  // enable clock
  FTM0_SC = FTM_SC_CLKS(0b01)      // use system clock (60 MHz - half of processor speed)
              | FTM_SC_PS(0b101);  // prescaler - divide by 32
}

void motors_command_all_off() {
  zero_outputs();
  _motors_command(); // skip checks, just turn them off
}

void motors_initialize_pin(uint8_t pin) {
  *portConfigRegister(pin) |= PORT_PCR_MUX(4); // set pins to FTM0 mode
  *portConfigRegister(pin) |= PORT_PCR_PE;     // pull enable
  *portConfigRegister(pin) &= ~PORT_PCR_PS;    // pull select -> pulldown
}

void motors_init() {
  zero_outputs();

  FTM0_SC = 0x0; // set status/control to zero = disabled
  FTM0_CNT = 0x0; // reset count to zero
  FTM0_MOD = 0xFFFF; // max modulus (we never want to reach this)

  motors_command_all_off();

  // PORT Configuration
  motors_initialize_pin(M1_PIN);
  motors_initialize_pin(M2_PIN);
  motors_initialize_pin(M3_PIN);
  motors_initialize_pin(M4_PIN);
}
