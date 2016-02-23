import * as t from '../action_types'

const metaInitialState = {
  currentTab: 'GRAPHS'
}

export default function (state = metaInitialState, action) {
  const payload = action.payload

  switch (action.type) {
    case t.TAB_SELECTED:
      return { ...state, currentTab: payload }
    default:
      return state
  }
}
