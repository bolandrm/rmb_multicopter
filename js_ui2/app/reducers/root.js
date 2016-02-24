import { combineReducers } from 'redux'
import * as t from '../action_types'

import commsReducer from './commsReducer'
import metaReducer from './metaReducer'
import tabsReducer from './tabsReducer'

export default combineReducers({
  comms: commsReducer,
  meta: metaReducer,
  tabs: tabsReducer
})
