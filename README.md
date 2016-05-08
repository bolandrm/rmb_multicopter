# Arduino / Teensy Multicopter

This is my WIP custom arduino quadcopter firmware.

### Blog posts:

- [Reading Remote Control Receiver Values with Arduino](https://ryanboland.com/blog/reading-rc-receiver-values/)

### Media:

- [First Successful Test Flight](https://www.youtube.com/watch?v=e4rH2S3EOmw&feature=youtu.be)
- [Talk Describing Code](https://www.youtube.com/watch?v=CHSYgLfhwUo&t=1947)

## Hardware

- Teensy 3.2
- Chinese GY-87 breakout board (MPU6050 gyro/acc, MHC5883L magnetometer, BMP180 barometer)
- HobbyKing HK-T6A transmitter/receiver
- 3DR telemetry
- HobbyKing Spec FPV250 V2 Quad Copter ARF Combo Kit
- 12A Afro ESCs with [SimonK firmware](https://github.com/sim-/tgy/releases/tag/2015-09-12)
  (supporting oneshot125)

## Building

The Makefiles (located in the build_scripts directory) are based on the awesome
[Arduino-Makefile](https://github.com/sudar/Arduino-Makefile) project.  They are
modified for use with Teensy 3.2, [Teensyduino 1.28](https://www.pjrc.com/teensy/td_download.html)
and Arduino 1.6.8.

- `make` - build project
- `make burn` - builds project, sends reboot signal to the Teensy to put it into
  the bootloader mode, and then uses `teensy_loader_cli` to upload to the Teensy
  on the command line without any GUI applications.
- `make monitor` - open connection to Teensy serial port
- `make clean` - remove compiled files in build directories

## UI

The UI is an Atom application using ReactJS and node-serialport.
