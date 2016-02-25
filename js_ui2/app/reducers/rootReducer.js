import { combineReducers } from 'redux'
import { reducer as formReducer } from 'redux-form'

import * as t from '../action_types'

import commsReducer from './commsReducer'
import metaReducer from './metaReducer'
import tuningTabReducer from './tuningTabReducer'
import pidsTabReducer from './pidsTabReducer'

const tabsReducer = combineReducers({
  tuning: tuningTabReducer,
  pids: pidsTabReducer
})

export default combineReducers({
  comms: commsReducer,
  meta: metaReducer,
  tabs: tabsReducer,
  form: formReducer
})
