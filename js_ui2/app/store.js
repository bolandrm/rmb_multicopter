import _ from 'lodash'
import { createStore, applyMiddleware } from 'redux'
import createSagaMiddleware from 'redux-saga'
import createLogger from 'redux-logger'

import reducer from './reducer'
import rootSaga from './saga'
import * as t from './action_types'
import SerialCodes from './serial_codes'

const filteredActions = [
  'EFFECT_TRIGGERED',
  'EFFECT_RESOLVED',
  'EFFECT_REJECTED'
]

const logger = createLogger({
  predicate: (getState, action) => {
    if (filteredActions.indexOf(action.type) > -1 ||
        action.type === t.DATA_PARSED && action.payload.code === SerialCodes.REQUEST_GYRO_ACC ) {
      return false
    } else {
      return true
    }
  }
})

const sagaMiddleware = createSagaMiddleware(rootSaga)

const store = createStore(
  reducer,
  applyMiddleware(sagaMiddleware, logger) // logger should be last middleware
)

if (module.hot) {
  module.hot.accept('./reducer', () =>
    store.replaceReducer(require('./reducer'))
  );
}

export default store
