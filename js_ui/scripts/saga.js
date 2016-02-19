import _ from 'underscore'
import { take, put, call, fork } from 'redux-saga/effects'
import * as actions from './actions'
import * as t from './action_types'
import serial from './serial_manager'

const isDefaultDevice = function (device) {
  return device.match(/usbmodem/)
}

const findDefaultDevice = function (devices) {
  return _.detect(devices, (device) => isDefaultDevice(device))
}

const autoconnect = function *(getComms) {
  yield put(actions.refreshDevices())
  yield take(t.DEVICE_CHANGED)

  const selectedDevice = getComms().selectedDevice
  if (isDefaultDevice(selectedDevice)) yield put(actions.connect())
}

const connect = function *(getComms) {
  yield take(t.CONNECT)
  const deviceId = yield call(serial.connect, getComms().selectedDevice)
  yield put(actions.connected(deviceId))
}

const disconnect = function *() {
  yield take(t.DISCONNECT)
  const deviceId = yield call(serial.disconnect)
  yield put(actions.disconnected())
}

const refreshDevices = function *() {
  while (true) {
    yield take(t.REFRESH_DEVICES)
    const devices = yield call(serial.getDevices)
    yield put(actions.gotDevices(devices))

    const selectedDevice = findDefaultDevice(devices) || devices[0]
    yield put(actions.deviceChanged(selectedDevice))
  }
}

// export const refreshDevices = function (opts = {}) {
//   serial.getDevices((devices) => {
//     gotDevices(devices)
// 
//     var device;
//     var autoconnectDevice;
// 
//     if (opts.autoconnect) {
//       autoconnectDevice = getAutoconnectDevice(devices)
//       device = autoconnectDevice || devices[0];
//     } else {
//       device = devices[0];
//     }
// 
//     deviceChanged(device)
// 
//     if (opts.autoconnect && autoconnectDevice) connect()
//   })
// }

// const connect = function () {
//   busy()
// 
//   serial.connect(getState().comms.selectedDevice, function (portId) {
//     if (portId) {
//       connected(portId)
//     } else {
//       failedToConnect()
//     }
//   })
// }


//  function *autoconnect(getState) {
//  
//    const device = autoconnectgetState()
//    if (
//  
//  
//      const {fullName, requiredFields = []} = yield take(actions.LOAD_REPO_PAGE)
//  
//      yield fork(loadRepo, fullName, getRepo(fullName), requiredFields)
//      yield fork(loadStargazers, fullName, getStargazersByRepo(fullName))
//  }

export default function* root(getState) {
  yield fork(refreshDevices)
  yield fork(autoconnect, () => getState().comms)
  yield fork(connect, () => getState().comms)
  yield fork(disconnect)
}
