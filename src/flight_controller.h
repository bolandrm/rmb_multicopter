#ifndef FLIGHT_CONTROLLER_H
#define FLIGHT_CONTROLLER_H

#define RATE 0
#define STABILIZE 1

#define ARMED 1
#define UNARMED 0

void fc_init();
void fc_arm();
void fc_process();
void fc_emergency_stop();

#endif
