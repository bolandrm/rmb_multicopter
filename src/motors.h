#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>

#define ALLOW_MOTORS

#define PWM_FREQUENCY 244   // in Hz
#define PWM_PRESCALER 8
#define PWM_COUNTER_PERIOD (F_CPU/PWM_PRESCALER/PWM_FREQUENCY)

#define MOTOR_SAFE_OFF 600
#define MOTOR_MIN 1070
#define MOTOR_MAX 1860
#define INDOOR_SAFE_MOTOR_SPEED 1500 //5000 //1800

#define NUM_MOTORS 4
#define M1 0
#define M2 1
#define M3 2
#define M4 3

void motors_command();
void motors_command_all_off();
void motors_init();
void motors_set_output(int8_t motor_number, int16_t output);
int16_t motor_level(int8_t motor_number);

#endif
