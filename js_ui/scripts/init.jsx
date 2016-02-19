import React from "react"
import ReactDOM from 'react-dom'
import { Provider } from 'react-redux'

import _ from "underscore"
import d3 from "d3"

import serial from "./serial_manager"
import SerialCodes from "./serial_codes"
import App from "./components/app"
import * as actions from "./actions"
import store from "./store"

//assignAll(actions, store)
Object.keys(actions).reduce(function (assigns, action) {
  if (!_.isFunction(actions[action].assignTo)) return
  assigns[action] = actions[action].assignTo(store);
  return assigns;
}, {});

actions.setStore(store)

window.onload = function() {
  // for debugging
  window._serial = serial
  window._SerialCodes = SerialCodes
  window._d3 = d3
  window._store = store
  window._actions = actions

  ReactDOM.render(
    <Provider store={store}>
      <App />
    </Provider>,
    document.getElementById('app')
  )
}
