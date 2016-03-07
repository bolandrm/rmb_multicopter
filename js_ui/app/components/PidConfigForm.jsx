import React from 'react'
import PidRow from './PidConfigFormRow'
import { configStore } from '../store'
import serial from '../serialManager'
import { observer } from 'mobx-react'

@observer
class PidConfigForm extends React.Component {
  render() {
    const {
      data: { pid_rate_xy, pid_rate_z, pid_angle_xy },
      fetchConfig, writeConfig, fetchedOnce
    } = configStore

    return (
      <div className='pid-inputs form-inline'>
        <div className='pid-group'>
          <div className='row-title'>Rate PIDs</div>
          <PidRow pidField={pid_rate_xy} rowLabel='X/Y' />
          <PidRow pidField={pid_rate_z} rowLabel='Z' />
        </div>
        <div className='pid-group'>
          <div className='row-title'>Angle PIDs</div>
          <PidRow pidField={pid_angle_xy} rowLabel='X/Y' />
        </div>

        <div className='pid-group pid-group-buttons'>
          <button disabled={!serial.connected} onClick={fetchConfig} className='btn btn-default'>
            Fetch Config
          </button>
          <button disabled={!serial.connected || !fetchedOnce} onClick={writeConfig} className='btn btn-success'>
            Write Config
          </button>
        </div>
      </div>
    )
  }
}

export default PidConfigForm
