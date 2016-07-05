const axisFloatType = [
  [ "x", "f32" ],
  [ "y", "f32" ],
  [ "z", "f32" ]
]

export const gyroAcc = [
  [ "rates", axisFloatType ],
  [ "angles", axisFloatType ],
  [ "gyroAngles", axisFloatType ],
  [ "gyroRaws", axisFloatType ],
  [ "accelRaws", axisFloatType ],
  [ "accelFiltered", axisFloatType ],
  [ "accelAngles", axisFloatType ],
  [ "gyroMaxValue", "f32" ],
  [ "accelMaxValue", "f32" ]
]

export const flightData = [
  [ "armed", "uint8" ],
  [ "mode", "uint8" ],
  [ "loopTime", "uint32" ],
  [ "batteryVoltage", "f32" ],
]

export const motorData = [
  [ "m1", "uint16" ],
  [ "m2", "uint16" ],
  [ "m3", "uint16" ],
  [ "m4", "uint16" ]
]

const rcChannelData = [
  [ "function", "uint8" ],
  [ "rawValue", "uint16" ],
  [ "value", "f32" ],
  [ "valueMin", "f32" ],
  [ "valueMax", "f32" ]
]

export const rcData = [
  [ "ch1", rcChannelData ],
  [ "ch2", rcChannelData ],
  [ "ch3", rcChannelData ],
  [ "ch4", rcChannelData ],
  [ "ch5", rcChannelData ],
  [ "ch6", rcChannelData ]
]

const pidData = [
  [ "setpoint", "f32" ],
  [ "input", "f32" ],
  [ "output", "f32" ],
  [ "pTerm", "f32" ],
  [ "iTerm", "f32" ],
  [ "dTerm", "f32" ]
]

export const ratePidData = [
  [ "pidRateX", pidData ],
  [ "pidRateY", pidData ],
  [ "pidRateZ", pidData ]
]

const pidConfig = [
  [ "kp", "f32" ],
  [ "ki", "f32" ],
  [ "imax", "f32" ],
  [ "kd", "f32" ]
]

const rcConfig = [
  [ "function", "uint8" ],
  [ "min", "uint16" ],
  [ "max", "uint16" ],
  [ "invert", "uint8" ],
]

export const config = [
  [ "version", "uint16" ],

  [ "ch1", rcConfig ],
  [ "ch2", rcConfig ],
  [ "ch3", rcConfig ],
  [ "ch4", rcConfig ],
  [ "ch5", rcConfig ],
  [ "ch6", rcConfig ],

  [ "pid_rate_xy", pidConfig ],
  [ "pid_rate_xy", pidConfig ],
  [ "pid_rate_z", pidConfig ],
  [ "pid_angle_xy", pidConfig ],
  [ "pid_angle_xy", pidConfig ]
]
