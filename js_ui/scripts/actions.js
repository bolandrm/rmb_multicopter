import _ from "underscore";
import { createAction } from 'redux-act'
import store from './store'
import serial from './serial_manager'

export const busy = createAction()
export const failedToConnect = createAction()
export const connected = createAction()
export const disconnected = createAction()
export const deviceChanged = createAction()

export const toggleConnection = function () {
  const comms = store.getState().comms
  if (comms.busy) return

  if (comms.connected) {
    disconnect()
  } else {
    connect()
  }
}

export const refreshDevices = function (opts = {}) {
  serial.getDevices((devices) => {
    var device;
    var autoconnectDevice;

    if (opts.autoconnect) {
      autoconnectDevice = getAutoconnectDevice(devices)
      device = autoconnectDevice || devices[0];
    } else {
      device = devices[0];
    }

    console.log(device);

    deviceChanged(device)

    if (opts.autoconnect && autoconnectDevice) {
      connect(autoconnectDevice)
    }
  })
}

const getAutoconnectDevice = function (devices) {
  _.detect(devices, (device) => device.match(/usbmodem/))
}

const connect = function (port) {
  busy()

  serial.connect(port, function(portId) {
    if (portId) {
      connected(portId)
    } else {
      failedToConnect()
    }
  })
}

const disconnect = function () {
  busy()
  serial.disconnect(disconnected)
}
