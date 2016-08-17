#include "logger.h"
#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

// #include "imu.h"
// #include "utils.h"
// #include "pids.h"
// #include "remote_control.h"
// #include "motors.h"
// #include "flight_controller.h"
// #include "serial_commands.h"
// #include "battery_monitor.h"

bool logger_active = false;

void logger_init() {
  Serial.print("Initializing SD card...");

  if (!SD.begin(LOGGER_SPI2_CHIP_SELECT)) {
    Serial.println("Card failed, or not present");
    return;
  }

  logger_active = true;
  Serial.println("card initialized.");
}

void logger_log() {
  if (!logger_active) return;

  // make a string for assembling the data to log:
  String dataString = "";

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  //  dataFile.close();
  //
  //  dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}
