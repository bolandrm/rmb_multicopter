import React from 'react'

class Toolbar extends React.Component {
  deviceChanged = (e) => {
    this.props.actions.deviceChanged(e.target.value)
  }

  tabSelected = (tab) => {
    this.props.actions.tabSelected(tab)
  }

  tabData = [
    { key: 'TUNING', text: 'Tuning' },
    { key: 'PIDS', text: 'PIDs' },
    { key: 'RC', text: 'Remove Control' },
    { key: 'IMU', text: 'IMU' },
    { key: 'MISC', text: 'Misc' },
  ]

  tabClass(tab) {
    return this.props.currentTab === tab ? 'active' : ''
  }

  render() {
    const actions = this.props.actions

    return (
      <div className='well toolbar'>
        <ul className="nav nav-tabs">
          {this.tabData.map((tab, i) => {
            return (
              <li className={this.tabClass(tab.key)} key={i}>
                <a href="#" onClick={this.tabSelected.bind(this, tab.key)}>
                  {tab.text}
                </a>
              </li>
            )
          })}
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
