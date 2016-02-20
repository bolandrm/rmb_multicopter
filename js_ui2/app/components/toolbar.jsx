import React from "react"

class Toolbar extends React.Component {
  deviceChanged = (e) => {
    this.props.actions.deviceChanged(e.target.value)
  }

  render() {
    const actions = this.props.actions

    return (
      <div className="toolbar">
        <label>
          Port:
          <select onChange={this.deviceChanged} value={this.props.selectedDevice}>
            {this.props.devices.map((device, i) => {
              return <option key={i}>{device}</option>
            })}
          </select>
        </label>
        <button onClick={actions.refreshDevices}>Refresh</button>
        <button onClick={actions.toggleConnection}>
          {this.props.connected ? "Disconnect" : "Connect"}
        </button>
      </div>
    )
  }
}

export default Toolbar
