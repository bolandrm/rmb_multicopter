export default {
  gyroAcc: [
    [ 
      "rates", [
        [ "x", "f" ],
        [ "y", "f" ],
        [ "z", "f" ]
      ]
    ],
    [ 
      "angles", [
        [ "x", "f" ],
        [ "y", "f" ],
        [ "z", "f" ]
      ]
    ],
    [ 
      "gyroAngles", [
        [ "x", "f" ],
        [ "y", "f" ],
        [ "z", "f" ]
      ]
    ],
    [ 
      "gyroRaws", [
        [ "x", "i" ],
        [ "y", "i" ],
        [ "z", "i" ]
      ]
    ],
    [ 
      "accelRaws", [
        [ "x", "i" ],
        [ "y", "i" ],
        [ "z", "i" ]
      ]
    ],
    [ 
      "accelFiltered", [
        [ "x", "f" ],
        [ "y", "f" ],
        [ "z", "f" ]
      ]
    ],
    [ 
      "accelAngles", [
        [ "x", "f" ],
        [ "y", "f" ],
        [ "z", "f" ]
      ]
    ],
    [ "gyroMaxValue", "f" ],
    [ "accelMaxValue", "f" ]
  ],

  config: [
    [ "version", "H" ],

    [ 
      "pid_rate_xy", [
        [ "kp", "f" ],
        [ "ki", "f" ],
        [ "imax", "f" ]
      ]
    ],
    [ 
      "pid_rate_xy", [
        [ "kp", "f" ],
        [ "ki", "f" ],
        [ "imax", "f" ]
      ]
    ],
    [
      "pid_rate_z", [
        [ "kp", "f" ],
        [ "ki", "f" ],
        [ "imax", "f" ]
      ]
    ],
    [
      "pid_angle_xy", [
        [ "kp", "f" ],
        [ "ki", "f" ],
        [ "imax", "f" ]
      ]
    ],
    [
      "pid_angle_xy", [
        [ "kp", "f" ],
        [ "ki", "f" ],
        [ "imax", "f" ]
      ]
    ]
  ]
};
