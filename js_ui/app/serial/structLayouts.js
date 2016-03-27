const axisFloatType = [
  [ "x", "f" ],
  [ "y", "f" ],
  [ "z", "f" ]
]

export const gyroAcc = [
  [ "rates", axisFloatType ],
  [ "angles", axisFloatType ],
  [ "gyroAngles", axisFloatType ],
  [ "gyroRaws", axisFloatType ],
  [ "accelRaws", axisFloatType ],
  [ "accelFiltered", axisFloatType ],
  [ "accelAngles", axisFloatType ],
  [ "gyroMaxValue", "f" ],
  [ "accelMaxValue", "f" ]
]

const rcChannelData = [
  [ "function", "B" ],
  [ "rawValue", "H" ],
  [ "value", "f" ],
  [ "valueMin", "f" ],
  [ "valueMax", "f" ]
]

export const rcData = [
  [ "ch1", rcChannelData ],
  [ "ch2", rcChannelData ],
  [ "ch3", rcChannelData ],
  [ "ch4", rcChannelData ],
  [ "ch5", rcChannelData ],
  [ "ch6", rcChannelData ]
]

const pidConfig = [
  [ "kp", "f" ],
  [ "ki", "f" ],
  [ "imax", "f" ]
]

const rcConfig = [
  [ "function", "B" ],
  [ "min", "H" ],
  [ "max", "H" ],
  [ "invert", "B" ],
]

export const config = [
  [ "version", "H" ],

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
