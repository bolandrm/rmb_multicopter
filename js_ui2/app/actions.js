import * as t from "./action_types"

function createAction(type, mapper) {
  let action = { type }

  if (mapper) {
    return function(...args) {
      return { type, payload: mapper(args) }
    }
  } else {
    return function(payload) {
      return { type, payload }
    }
  }
}

export const deviceChanged = createAction(t.DEVICE_CHANGED)
export const gotDevices = createAction(t.GOT_DEVICES)
export const connect = createAction(t.CONNECT)
export const connected = createAction(t.CONNECTED)
export const failedToConnect = createAction(t.FAILED_TO_CONNECT)
export const disconnected = createAction(t.DISCONNECTED)

export const refreshDevices = createAction(t.REFRESH_DEVICES)
export const toggleConnection = createAction(t.TOGGLE_CONNECTION)

// export const busy = createAction('busy')
// export const failedToConnect = createAction('failedToConnect')
// export const connected = createAction('connected')
// export const disconnected = createAction('disconnected')
// export const gotDevices = createAction('gotDevices')
// export const deviceChanged = createAction('deviceChanged')
// 
// var _store = null
// 
// const getState = () => {
//   return _store.getState()
// }
// 
// export const setStore = function (store) {
//   _store = store
// }
// 
// export const toggleConnection = function () {
//   const comms = getState().comms
//   if (comms.busy) return
// 
//   if (comms.connected) {
//     disconnect()
//   } else {
//     connect()
//   }
// }
// 
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
// 
// const disconnect = function () {
//   busy()
//   serial.disconnect(getState().comms.openPortId, disconnected)
// }
