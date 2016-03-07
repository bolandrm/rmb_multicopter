import 'babel-polyfill'
import React from 'react'
import ReactDOM from 'react-dom'
import App from './components/app'

import './styles/app.scss'

// for debugging
import * as stores from './store'
import _ from 'lodash'
import d3 from 'd3'
import serial from './serialManager'

window.__serial = serial
window.__d3 = d3
window.__stores = stores

process.on('unhandledRejection', (reason, p) => {
  console.log('Unhandled Rejection at: Promise ', p, ' reason: ', reason)
})

ReactDOM.render(
  <App />,
  document.getElementById('app')
)
