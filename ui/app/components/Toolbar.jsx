import React from 'react'
import { observer } from 'mobx-react'
import serial from '../serial/serialManager'
import { metaStore } from '../store'

@observer
class Toolbar extends React.Component {
  portChanged = (e) => {
    serial.portSelected = e.target.value
  }

  tabClass(tab) {
    return metaStore.currentTab === tab ? 'active' : ''
  }

  render() {
    return (
      <div className='well toolbar'>
        <ul className="nav nav-tabs">
          {metaStore.tabData.map((tab, i) => {
            return (
              <li className={this.tabClass(tab.key)} key={i}>
                <a href="#" onClick={metaStore.tabSelected.bind(metaStore, tab.key)}>
                  {tab.text}
                </a>
              </li>
            )
          })}
        </ul>

        <div className='connection form-inline'>
          <select className='form-control' disabled={serial.connected || serial.busy}
            onChange={this.portChanged} value={serial.portSelected}>
            {serial.ports.map((device, i) => {
              return <option key={i}>{device}</option>
            })}
          </select>

          {!serial.connected && !serial.busy &&
            <button className='btn btn-default' onClick={serial.refreshPorts}>Refresh</button>
          }

          {serial.connected ?
            <button className='btn btn-success' onClick={serial.disconnect} disabled={serial.busy}>
              Disconnect
            </button>
          :
            <button className='btn btn-primary' onClick={serial.connect} disabled={serial.busy}>
              Connect
            </button>
          }
        </div>
      </div>
    )
  }
}

export default Toolbar
