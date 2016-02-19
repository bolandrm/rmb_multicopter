import { combineReducers } from 'redux'
import * as t from "./action_types"

const commsInitialState = {
  selectedDevice: null,
  busy: false,
  devices: [],
  connected: false
}

const comms = function (state = commsInitialState, action) {
  console.log("Action triggered", action.type, action.payload)

  const payload = action.payload

  switch (action.type) {
    case t.DEVICE_CHANGED:
      return { ...state, selectedDevice: payload }
    case t.GOT_DEVICES:
      return { ...state, devices: payload }
    case t.CONNECT:
    case t.DISCONNECT:
      return { ...state, busy: true }
    case t.CONNECTED:
      return { ...state, busy: false, connected: true, openPortId: payload }
    case t.FAILED_TO_CONNECT:
      return { ...state, busy: false, connected: false }
    case t.DISCONNECTED:
      return { ...state, busy: false, connected: false, openPortId: null }
    default:
      return state
  }
}

export default combineReducers({comms})
