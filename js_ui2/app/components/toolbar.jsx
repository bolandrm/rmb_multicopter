import React from 'react'

class Toolbar extends React.Component {
  deviceChanged = (e) => {
    this.props.actions.deviceChanged(e.target.value)
  }

  render() {
    const actions = this.props.actions

    return (
      <div className='well toolbar'>
        <ul className="nav nav-tabs">
          <li className="active"><a href="#">Graphs</a></li>
          <li><a href="#">PIDs</a></li>
          <li><a href="#">Remote Control</a></li>
          <li><a href="#">IMU</a></li>
          <li><a href="#">Scheduler</a></li>
        </ul>

        <div className='connection form-inline'>
          <select className='form-control' disabled={this.props.connected || this.props.busy}
            onChange={this.deviceChanged} value={this.props.selectedDevice}>
            {this.props.devices.map((device, i) => {
              return <option key={i}>{device}</option>
            })}
          </select>

          {!this.props.connected && !this.props.busy &&
            <button className='btn btn-default' onClick={actions.refreshDevices}>Refresh</button>
          }

          {this.props.connected ?
            <button className='btn btn-success' onClick={actions.disconnect} disabled={this.props.busy}>
              Disconnect
            </button>
          :
            <button className='btn btn-primary' onClick={actions.connect} disabled={this.props.busy}>
              Connect
            </button>
          }
        </div>
      </div>
    )
  }
}

export default Toolbar
