import React from 'react';
import PidConfigForm from './PidConfigForm'
import { observer } from 'mobx-react'

@observer
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
