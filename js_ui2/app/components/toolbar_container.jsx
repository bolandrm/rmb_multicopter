import React from 'react'
import { connect } from 'react-redux'
import { bindActionCreators } from 'redux'
import * as actions from '../actions'
import Toolbar from "./toolbar"

const ToolbarContainer = connect(
  (state) => {
    return {
      selectedDevice: state.comms.selectedDevice,
      busy: state.comms.busy,
      connected: state.comms.connected,
      devices: state.comms.devices
    }
  },

  (dispatch) => {
    return { actions: bindActionCreators(actions, dispatch) }
  }
)(Toolbar)

export default ToolbarContainer
