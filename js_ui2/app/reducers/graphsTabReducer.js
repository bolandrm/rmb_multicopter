import Utils from '../utils'
import * as t from '../action_types'

const tabReducers = {}

const graphTypes = {
  'angleXFusion': ['accelAngles.x', 'gyroAngles.x', 'angles.x']
}

const initialGraphData = function(type) {
  const data = graphTypes[type].map((key) => {
    return { key: key, samples: [], filtered: false }
  })

  return { type, data, sampleCount: 200 }
}

const graphsTabInitialState = {
  graph1: initialGraphData('angleXFusion')
}

const newGraphData = function (state, payload) {
  var shift = (state.data[0].samples.length >= state.sampleCount) ? 1 : 0

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

