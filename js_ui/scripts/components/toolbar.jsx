import _ from "underscore";
import React from "react";
import serial from "../serial_manager";

class Toolbar extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      devices: [],
      deviceSelected: ""
    };
    this.refreshDevices(true);
  }

  deviceChanged = (e) => {
    this.setState({deviceSelected: e.target.value});
  }

  toggleConnection = (e) => {
    if (this.props.connected) {
      serial.disconnect();
    } else {
      serial.connect(this.state.deviceSelected);
    }
  }

  refreshDevices = (connectToDefault = false) => {
    serial.getDevices((devices) => {
      var device;
      var autoConnectDevice;

      if (connectToDefault) {
        autoConnectDevice = _.detect(devices, function (device) {
          return device.match(/usbmodem/);
        });
          device = autoConnectDevice || devices[0];
      } else {
        device = devices[0];
      }

      console.log(device);

      this.setState({ devices: devices, deviceSelected: device }, () => {
        if (connectToDefault && autoConnectDevice) {
          console.log("attempting autoconnect");
          this.toggleConnection();
        }
      });
    });
  }

  render() {
    return(
      <div className="toolbar">
        <label>
          Port:
          <select onChange={this.deviceChanged} value={this.state.deviceSelected}>
            {this.state.devices.map((device, i) => {
              return <option key={i}>{device}</option>;
            })}
          </select>
        </label>
        <button onClick={this.refreshDevices}>Refresh</button>
        <button onClick={this.toggleConnection}>
          {this.props.connected ? "Disconnect" : "Connect"}
        </button>
      </div>
    );
  }
}

export default Toolbar;
