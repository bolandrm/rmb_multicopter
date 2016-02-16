import React from "react";
import ReactDOM from 'react-dom';
import d3 from "d3";
import serial from "./serial_manager";
import SerialCodes from "./serial_codes";
import App from "./components/app";

window.onload = function() {
  // for debugging
  window.serial = serial;
  window.SerialCodes = SerialCodes;
  window.d3 = d3;

  ReactDOM.render(
    React.createElement(App),
    document.getElementById('app')
  );
};
