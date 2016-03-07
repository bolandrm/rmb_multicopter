import * as t from '../action_types'
import moment from 'moment'

const metaInitialState = {
  currentTab: 'TUNING',
  remoteConfigData: {},
  consoleMessages: []
}

export default function (state = metaInitialState, action) {
  const payload = action.payload

  switch (action.type) {
    case t.TAB_SELECTED:
      return { ...state, currentTab: payload }
    case t.SERIAL_GOT_CONFIG:
      const time = moment().format("HH:mm:ss.SSS")

      return { 
        ...state,
        remoteConfigData: payload,
        consoleMessages: [
          { text: "Config data loaded", time },
          ...state.consoleMessages.slice(0, 29)
        ]
      }
    default:
      return state
  }
}
