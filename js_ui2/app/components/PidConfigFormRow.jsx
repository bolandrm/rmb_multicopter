import { observer } from 'mobx-react'
import React from 'react'

@observer
class PidConfigFormRow extends React.Component {
  fieldProps(field) {
    return({
      value: this.props.pidField[field],
      onChange: (e) => this.props.pidField[field] = e.target.value
    })
  }

  render() {
    return (
      <div className='pid-row'>
        <div className='row-label'>
          {this.props.rowLabel}
        </div>
        <div className='form-group'>
          <label>P</label>
          <input type='text' className='form-control' {...this.fieldProps('kp')} />
        </div>
        <div className='form-group'>
          <label>I</label>
          <input type='text' className='form-control' {...this.fieldProps('ki')} />
        </div>
        <div className='form-group'>
          <label>I Max</label>
          <input type='text' className='form-control' {...this.fieldProps('imax')} />
        </div>
      </div>
    )
  }
}

export default PidConfigFormRow
