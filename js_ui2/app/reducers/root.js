import { combineReducers } from 'redux'
import * as t from '../action_types'

import commsReducer from './commsReducer'
import metaReducer from './metaReducer'
import graphsTabReducer from "./graphsTabReducer"
import pidsTabReducer from "./pidsTabReducer"

const tabsReducer = combineReducers({
  graphs: graphsTabReducer,
  pids: pidsTabReducer
})

export default combineReducers({
  comms: commsReducer,
  meta: metaReducer,
  tabs: tabsReducer
})
