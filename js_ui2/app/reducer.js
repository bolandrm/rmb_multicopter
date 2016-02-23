import { combineReducers } from 'redux'
import * as t from './action_types'
import SerialCodes from "./serial_codes";
import Utils from "./utils";

const tabReducers = {}

const graphTypes = {
  "angleXFusion": ["accelAngles.x", "gyroAngles.x", "angles.x"]
}

const initialGraphData = function(type) {
  const data = graphTypes[type].map((key) => {
    return { key: key, samples: [], filtered: false }
  })

  return { type, data }
}

const graphsTabInitialState = {
  graph1: initialGraphData('angleXFusion')
}

const SAMPLE_COUNT = 200

const newGraphData = function (state, payload) {
  var shift = (state.data[0].samples.length >= SAMPLE_COUNT) ? 1 : 0

  const data = state.data.map((series, i) => {
    return {
      ...series,
      samples: [ 
        ...series.samples.slice(shift),
        Utils.deepFetch(payload.data, series.key)
      ]
    }
  })

  return { ...state, data }
}

tabReducers.graphs = function (state = graphsTabInitialState, action) {
  const payload = action.payload

  switch (action.type) {
    case t.DATA_PARSED:
      return {
        ...state,
        graph1: newGraphData(state.graph1, payload)
      }
    case t.GRAPH_FILTERED:
      if (!state[payload.graph].data.find((s) => s.key === payload.key).filtered &&
          state[payload.graph].data.filter((series) => !series.filtered).length === 1)
        return state

      return {
        ...state,
        [payload.graph]: {
          ...state[payload.graph],
          data: state[payload.graph].data.map((series) => {
            return {
              ...series,
              filtered: (series.key === payload.key ? !series.filtered : series.filtered)
            }
          })
        }
      }
    default:
      return state
  }
}

const tabs = combineReducers({graphs: tabReducers.graphs})

const commsInitialState = {
  selectedDevice: null,
  busy: false,
  devices: [],
  connected: false,
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

export default combineReducers({comms, meta, tabs})
