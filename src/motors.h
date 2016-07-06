#pragma once

#include <Arduino.h>

#define ALLOW_MOTORS

#define MOTOR_SAFE_OFF 1020
#define MOTOR_MIN 1070
#define MOTOR_MAX 1860
#define INDOOR_SAFE_MOTOR_SPEED 10000 //5000 //1800

#define NUM_MOTORS 4
#define M1 0
#define M2 1
#define M3 2
#define M4 3

#define M1_ON
#define M2_ON
#define M3_ON
#define M4_ON

#define M1_PIN 20
#define M2_PIN 21
#define M3_PIN 22
#define M4_PIN 23

#define M1_OUTPUT_REG FTM0_C5V
#define M2_OUTPUT_REG FTM0_C6V
#define M3_OUTPUT_REG FTM0_C0V
#define M4_OUTPUT_REG FTM0_C1V

#define utils_round_positive(n) ((uint16_t)((n) + 0.5))

#define PRESCALE_FACTOR  32
#define PERIPHERAL_CLOCK_SPEED  (F_CPU / 2)
#define FLEX_TIMER_TICK_IN_HZ  (PERIPHERAL_CLOCK_SPEED / PRESCALE_FACTOR)
#define FLEX_TIMER_TICK_IN_MICROSECONDS  ((1.0 / FLEX_TIMER_TICK_IN_HZ) * 1e6)
#define MOTOR_VALUE_SCALE_FACTOR  (1.0 / FLEX_TIMER_TICK_IN_MICROSECONDS)

#define REFRESH_RATE_IN_MICROSECONDS 2500  # 400 hz
#define REFRESH_RATE_MODULUS_F (2500 / FLEX_TIMER_TICK_IN_MICROSECONDS)
#define REFRESH_RATE_MODULUS (utils_round_positive(REFRESH_RATE_MODULUS_F))

void motors_command();
void motors_command_all_off();
void motors_init();
void motors_set_output(int8_t motor_number, int16_t output);
int16_t motor_level(int8_t motor_number);
