import React from 'react'
import { observer } from 'mobx-react'
import { connect } from 'react-redux'
import { bindActionCreators } from 'redux'
import * as actions from '../actions'
import TuningTab from './TuningTab'
import PidsTab from './PidsTab'

@observer
class TabContentContainer extends React.Component {
  render() {
    return (
      <div className='tab-container'>
        <div>{testStore.tester}</div>
      </div>
    )
  }
}

export default TabContentContainer

// {this.props.currentTab === 'TUNING' &&
//         <TuningTab {...this.props.tuningTab} actions={this.props.actions} />
//        }

//         {this.props.currentTab === 'PIDS' &&
//            <PidsTab {...this.props.pidsTab} actions={this.props.actions} />
//           }


// export default connect(
//      (state) => {
//       const tabs = state.tabs
// 
//     return {
//       testStore: testStore,
//       currentTab: state.meta.currentTab,
//       tuningTab: {
//         graph1: tabs.tuning.graph1
//       },
//       pidsTab: {
//       }
//     }
//   },
// 
//   (dispatch) => {
//     return { actions: bindActionCreators(actions, dispatch) }
//   }
// )(TabContentContainer)
