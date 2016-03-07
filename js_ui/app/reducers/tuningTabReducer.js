import * as t from '../action_types'
import { initialGraphData, lineGraphReducer } from './lineGraphReducer'

const tuningTabInitialState = {
  graph1: initialGraphData('angleYFusion')
}

export default function (state = tuningTabInitialState, action) {
  const payload = action.payload

  switch (action.type) {
    case t.SERIAL_GOT_GYRO_ACC:
    case t.GRAPH_FILTERED:
      return {
        ...state,
        graph1: lineGraphReducer(state.graph1, action)
      }
    default:
      return state
  }
}
