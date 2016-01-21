typedef struct __attribute__((packed)) {
  float kp;
  float ki;
  float kd;
  float i_max;
} pid_config;

typedef struct __attribute__((packed)) {
  uint16_t version;

  pid_config pid_rate_x;
  pid_config pid_rate_y;
  pid_config pid_rate_z;

  pid_config pid_angle_x;
  pid_config pid_angle_y;
  pid_config pid_angle_z;

} CONFIG_struct;

typedef union {
  CONFIG_struct data;
  uint8_t raw[sizeof(CONFIG_struct)];
} CONFIG_union;
