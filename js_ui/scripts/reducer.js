import { combineReducers } from 'redux'
import { createReducer } from 'redux-act'
import * as actions from './actions'

const comms = createReducer({
  [actions.deviceChanged]: (state, device) => {
    return { ...state, selectedDevice: device }
  },

  [actions.busy]: (state) => {
    return { ...state, busy: true }
  },

  [actions.gotDevices]: (state, devices) => {
    return { ...state, devices: devices }
  },

  [actions.failedToConnect]: (state) => {
    return { ...state, busy: false, connected: false }
  },

  [actions.connected]: (state, portId) => {
    return { ...state, busy: false, connected: true, openPortId: portId }
  },

  [actions.disconnected]: (state) => {
    return { ...state, busy: false, connected: false, openPortId: null }
  },
},
{
  selectedDevice: null,
  busy: false,
  devices: [],
  connected: false
});

export default combineReducers({comms})
