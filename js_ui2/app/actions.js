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
export const disconnect = createAction(t.DISCONNECT)
export const disconnected = createAction(t.DISCONNECTED)
export const refreshDevices = createAction(t.REFRESH_DEVICES)
export const toggleConnection = createAction(t.TOGGLE_CONNECTION)
export const dataParsed = createAction(t.DATA_PARSED)

export const graphFiltered = createAction(t.GRAPH_FILTERED)

export const tabSelected = createAction(t.TAB_SELECTED)
