import React from 'react'

class PidConfigFormRow extends React.Component {
  render() {
    const { pidField, rowLabel } = this.props

    return (
      <div className='pid-row'>
        <div className='row-label'>
          {rowLabel}
        </div>
        <div className='form-group'>
          <label>P</label>
          <input type='text' className='form-control' {...pidField.kp} />
        </div>
        <div className='form-group'>
          <label>I</label>
          <input type='text' className='form-control' {...pidField.ki} />
        </div>
        <div className='form-group'>
          <label>I Max</label>
          <input type='text' className='form-control' {...pidField.imax} />
        </div>
      </div>
    )
  }
}

export default PidConfigFormRow
