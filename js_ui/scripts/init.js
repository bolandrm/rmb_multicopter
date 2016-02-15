import codes from "./serial_codes";
import serial from "./serial_manager";

var cfg = {};

window.onload = function() {
  window.serial = serial;
  console.log(`working ${codes.SET_CONFIG}`);
};

console.log("working22");
