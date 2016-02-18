import _ from "underscore"
import React from "react"
import serial from "../serial_manager"

class Toolbar extends React.Component {
  render() {
    return(
      <div className="toolbar">
        <label>
          Port:
          <select onChange={this.props.deviceChanged} value={this.props.selectedDevice}>
            {this.props.devices.map((device, i) => {
              return <option key={i}>{device}</option>
            })}
          </select>
        </label>
        <button onClick={this.props.refreshDevices}>Refresh</button>
        <button onClick={this.props.toggleConnection}>
          {this.props.connected ? "Disconnect" : "Connect"}
        </button>
      </div>
    )
  }
}

export default Toolbar
