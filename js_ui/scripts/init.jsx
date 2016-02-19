import 'babel-polyfill'
import React from "react"
import ReactDOM from 'react-dom'
import { Provider } from 'react-redux'
import App from "./components/app"

// for debugging
import * as actions from "./actions"
import store from "./store"
import _ from "underscore"
import d3 from "d3"
import serial from "./serial_manager"
import SerialCodes from "./serial_codes"

window.onload = function() {
  // for debugging
  window.__serial = serial
  window.__SerialCodes = SerialCodes
  window.__d3 = d3
  window.__store = store
  window.__actions = actions

  ReactDOM.render(
    <Provider store={store}>
      <App />
    </Provider>,
    document.getElementById('app')
  )
}
