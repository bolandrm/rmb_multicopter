import { combineReducers } from 'redux'
import * as t from './action_types'

const commsInitialState = {
  selectedDevice: null,
  busy: false,
  devices: [],
  connected: false
}

const comms = function (state = commsInitialState, action) {
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
      return { ...state, busy: false, connected: true }
    case t.FAILED_TO_CONNECT:
      return { ...state, busy: false, connected: false }
    case t.DISCONNECTED:
      return { ...state, busy: false, connected: false }
    default:
      return state
  }
}

const metaInitialState = {
  currentTab: 'GRAPHS'
}

const meta = function (state = metaInitialState, action) {
  const payload = action.payload

  switch (action.type) {
    case t.TAB_SELECTED:
      return { ...state, currentTab: payload }
    default:
      return state
  }
}

export default combineReducers({comms, meta})
