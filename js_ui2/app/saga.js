import _ from 'lodash'
import { take, put, call, fork } from 'redux-saga/effects'
import * as actions from './actions'
import * as t from './action_types'
import serial from './serial_manager'
import SerialCodes from './serial_codes'

const isDefaultDevice = function (device) {
  return device.match(/usbmodem/)
}

const findDefaultDevice = function (devices) {
  return _.find(devices, (device) => isDefaultDevice(device))
}

const delay = function (delayMs) {
  return new Promise(function(resolve) {
    setTimeout(resolve, delayMs)
  })
}

const autoconnect = function *(getComms) {
  yield put(actions.refreshDevices())
  yield take(t.DEVICE_CHANGED)

  const selectedDevice = getComms().selectedDevice
  if (isDefaultDevice(selectedDevice)) yield put(actions.connect())
}

const connect = function *(getComms) {
  while (true) {
    yield take(t.CONNECT)
    const connected = yield call(serial.connect, getComms().selectedDevice)

    if (connected) {
      yield put(actions.connected())
    } else {
      yield put(actions.failedToConnect())
    }
  }
}

const disconnect = function *() {
  while (true) {
    yield take(t.DISCONNECT)
    yield call(serial.disconnect)
    yield put(actions.disconnected())
  }
}

const periodicDataFetch = function *(getState) {
  while (true) {
    if (!getState().comms.connected) {
      yield call(delay, 400)
      continue
    }

    if (getState().meta.currentTab === 'GRAPHS') {
      yield call(serial.send, SerialCodes.REQUEST_GYRO_ACC)
      yield call(delay, 50)
    } else {
      yield call(delay, 400)
    }
  }
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

export default function* root(getState) {
  yield fork(refreshDevices)
  yield fork(autoconnect, () => getState().comms)
  yield fork(connect, () => getState().comms)
  yield fork(disconnect)
  yield fork(periodicDataFetch, () => getState())
}
