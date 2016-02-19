import _ from 'underscore';
import { createAction } from 'redux-act'
import serial from './serial_manager'

export const busy = createAction('busy')
export const failedToConnect = createAction('failedToConnect')
export const connected = createAction('connected')
export const disconnected = createAction('disconnected')
export const gotDevices = createAction('gotDevices')
export const deviceChanged = createAction('deviceChanged')

var _store = null

const getState = () => {
  return _store.getState()
}

export const setStore = function (store) {
  _store = store
}

export const toggleConnection = function () {
  const comms = getState().comms
  if (comms.busy) return

  if (comms.connected) {
    disconnect()
  } else {
    connect()
  }
}

export const refreshDevices = function (opts = {}) {
  serial.getDevices((devices) => {
    gotDevices(devices)

    var device;
    var autoconnectDevice;

    if (opts.autoconnect) {
      autoconnectDevice = getAutoconnectDevice(devices)
      device = autoconnectDevice || devices[0];
    } else {
      device = devices[0];
    }

    deviceChanged(device)

    if (opts.autoconnect && autoconnectDevice) connect()
  })
}

const getAutoconnectDevice = function (devices) {
  return _.detect(devices, (device) => device.match(/usbmodem/))
}

const connect = function () {
  busy()

  serial.connect(getState().comms.selectedDevice, function (portId) {
    if (portId) {
      connected(portId)
    } else {
      failedToConnect()
    }
  })
}

const disconnect = function () {
  busy()
  serial.disconnect(getState().comms.openPortId, disconnected)
}
