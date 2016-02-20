import _ from 'lodash'
import { createStore, applyMiddleware } from 'redux'
import createSagaMiddleware from 'redux-saga'

import reducer from './reducer'
import rootSaga from './saga'

const sagaMiddleware = createSagaMiddleware(rootSaga)

const store = createStore(
  reducer,
  applyMiddleware(sagaMiddleware)
)

var previousStoreState = {}
store.subscribe(() => {
  if (!_.isEqual(previousStoreState, store.getState())) {
    previousStoreState = store.getState()
    console.log('store updated', store.getState())
  }
})

// if (module.hot) {
//   module.hot.accept('./reducer', () =>
//     store.replaceReducer(require('./reducer'))
//   );
// }

export default store
