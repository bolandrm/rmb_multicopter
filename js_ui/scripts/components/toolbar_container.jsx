import _ from 'underscore'
import React from 'react'
import { connect } from 'react-redux'
import { refreshDevices, deviceChanged, toggleConnection } from '../actions'
import serial from '../serial_manager'
import Toolbar from "./toolbar"

class ToolbarContainer extends React.Component {
  constructor(props) {
    super(props)
    console.log(props)
    console.log("hello")
    refreshDevices({autoconnect: true})
  }

  render() {
    return <Toolbar {...this.props} />
  }
}

export default connect(
  (state) => {
    return {
      selectedDevice: state.comms.selectedDevice,
      connected: state.comms.connected,
      devices: state.comms.devices
    }
  },

  () => {
    return {
      refreshDevices: refreshDevices,
      deviceChanged: deviceChanged,
      toggleConnection: toggleConnection
    }
  }
)(ToolbarContainer)

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

