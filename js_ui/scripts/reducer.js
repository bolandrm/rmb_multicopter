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

  [actions.failedToConnect]: (state, payload) => {
    return { ...state, busy: false, connected: false }
  },

  [actions.connected]: (state, payload) => {
    return { ...state, busy: false, connected: true }
  },

  [actions.disconnected]: (state, payload) => {
  },
},
{
  selectedDevice: null,
  busy: false,
  devices: [],
  connected: false
});

export default combineReducers({comms})
