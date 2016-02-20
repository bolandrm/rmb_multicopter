import React from 'react'
import { connect } from 'react-redux'
import { bindActionCreators } from 'redux'
import * as actions from '../actions'
import Toolbar from "./toolbar"

const ToolbarContainer = connect(
  (state) => {
    return {
      selectedDevice: state.comms.selectedDevice,
      connected: state.comms.connected,
      devices: state.comms.devices
    }
  },

  (dispatch) => {
    return { actions: bindActionCreators(actions, dispatch) }
  }
)(Toolbar)

export default ToolbarContainer

  //const deviceChanged = (e) => {
  //  this.setState({selectedDevice: e.target.value});
  //}

  //toggleConnection = (e) => {
  //  if (this.props.connected) {
  //    serial.disconnect();
  //  } else {
  //    serial.connect(this.state.selectedDevice);
  //  }
  //}

  //refreshDevices = (connectToDefault = false) => {
  //  serial.getDevices((devices) => {
  //    var device;
  //    var autoConnectDevice;

  //    if (connectToDefault) {
  //      autoConnectDevice = _.detect(devices, function (device) {
  //        return device.match(/usbmodem/);
  //      });
  //        device = autoConnectDevice || devices[0];
  //    } else {
  //      device = devices[0];
  //    }

  //    console.log(device);

  //    this.setState({ devices: devices, selectedDevice: device }, () => {
  //      if (connectToDefault && autoConnectDevice) {
  //        console.log('attempting autoconnect');
  //        this.toggleConnection();
  //      }
  //    });
  //  });
  //}

