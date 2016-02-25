import React from 'react';
import * as actions from '../actions'
import PidConfigForm from './PidConfigForm'

class PidsTab extends React.Component {
  render() {
    return (
      <div className='pids-tab'>
        <PidConfigForm />
      </div>
    )
  }
}

export default PidsTab
