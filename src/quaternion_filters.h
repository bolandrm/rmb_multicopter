#include "types.h"

void madgwick_quaternion_update(
  axis_float_t *angles,
  float deltat,
  float ax, float ay, float az,
  float gx, float gy, float gz
);
