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
        [ "x", "h" ],
        [ "y", "h" ],
        [ "z", "h" ]
      ]
    ],
    [ 
      "accelRaws", [
        [ "x", "h" ],
        [ "y", "h" ],
        [ "z", "h" ]
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
    ]
  ],

  config: [
    [ "version", "H" ],

    [ 
      "pid_rate_xy", [
        [ "kp", "f" ],
        [ "pMax", "f" ],
        [ "ki", "f" ],
        [ "iMax", "f" ]
      ]
    ],
    [ 
      "pid_rate_xy", [
        [ "kp", "f" ],
        [ "pMax", "f" ],
        [ "ki", "f" ],
        [ "iMax", "f" ]
      ]
    ],
    [
      "pid_rate_z", [
        [ "kp", "f" ],
        [ "pMax", "f" ],
        [ "ki", "f" ],
        [ "iMax", "f" ]
      ]
    ],
    [
      "pid_angle_xy", [
        [ "kp", "f" ],
        [ "pMax", "f" ],
        [ "ki", "f" ],
        [ "iMax", "f" ]
      ]
    ],
    [
      "pid_angle_xy", [
        [ "kp", "f" ],
        [ "pMax", "f" ],
        [ "ki", "f" ],
        [ "iMax", "f" ]
      ]
    ]
  ]
};
