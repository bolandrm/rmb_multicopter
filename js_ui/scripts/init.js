var cfg = {};

const onload = function() {
  cfg.serial = new cfg.Serial();
  cfg.serial.autoConnect();
};
