import React from 'react'
import { reduxForm } from 'redux-form'
import { bindActionCreators } from 'redux'
import * as actions from '../actions'
import PidRow from './PidConfigFormRow'

class PidConfigForm extends React.Component {
  render() {
    const {
      fields: { pid_rate_xy, pid_rate_z, pid_angle_xy },
      connected
    } = this.props

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
          <button disabled={!connected} className='btn btn-default'>Fetch Config</button>
          <button disabled={!connected} className='btn btn-success'>Write Config</button>
        </div>
      </div>
    )
  }
}

export default reduxForm(
  {
    form: 'config',
    fields: [
      'pid_rate_xy.kp',
      'pid_rate_xy.ki',
      'pid_rate_xy.imax',
      'pid_rate_z.kp',
      'pid_rate_z.ki',
      'pid_rate_z.imax',
      'pid_angle_xy.kp',
      'pid_angle_xy.ki',
      'pid_angle_xy.imax'
    ],
    destroyOnUnmount: false
  },

  (state) => {
    return { connected: state.comms.connected }
  },

  (dispatch) => {
    return { actions: bindActionCreators(actions, dispatch) }
  }

)(PidConfigForm)
