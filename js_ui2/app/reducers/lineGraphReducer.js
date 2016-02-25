import { deepFetch } from '../utils'
import * as t from '../action_types'
import SerialCodes from "../serial_codes";

const SAMPLE_COUNT = 200

export const graphTypes = {
  'angleXFusion': ['accelAngles.x', 'gyroAngles.x', 'angles.x'],
  'angleYFusion': ['accelAngles.y', 'gyroAngles.y', 'angles.y']
}

export const initialGraphData = function(type) {
  const data = graphTypes[type].map((key) => {
    return { key: key, samples: [], filtered: false }
  })

  return { type, data, sampleCount: SAMPLE_COUNT }
}

export const lineGraphReducer = function (state, action) {
  const payload = action.payload

  switch (action.type) {
    case t.SERIAL_GOT_GYRO_ACC:
      var shift = (state.data[0].samples.length >= state.sampleCount) ? 1 : 0

      const data = state.data.map((series, i) => {
        return {
          ...series,
          samples: [ 
            ...series.samples.slice(shift),
            deepFetch(payload, series.key)
          ]
        }
      })
      return { ...state, data }

    case t.GRAPH_FILTERED:
      if (!state.data.find((s) => s.key === payload.key).filtered &&
          state.data.filter((s) => !s.filtered).length === 1)
        return state

      return {
        ...state,
        data: state.data.map((s) => {
          const filtered = (s.key === payload.key ? !s.filtered : s.filtered)
          return { ...s, filtered: filtered }
        })
      }

    default:
      return state
  }
}
