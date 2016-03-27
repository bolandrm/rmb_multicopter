import React from 'react'
import { observer } from 'mobx-react'
import TuningTab from './TuningTab'
import PidsTab from './PidsTab'
import RCTab from './RCTab'
import { metaStore } from '../store'

@observer
class TabContentContainer extends React.Component {
  render() {
    return (
      <div className='tab-container'>
        {metaStore.currentTab === 'TUNING' &&
          <TuningTab />
        }
        {metaStore.currentTab === 'PIDS' &&
          <PidsTab />
        }
        {metaStore.currentTab === 'RC' &&
          <RCTab />
        }
      </div>
    )
  }
}

export default TabContentContainer
