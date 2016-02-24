import React from 'react'
import { connect } from 'react-redux'
import { bindActionCreators } from 'redux'
import * as actions from '../actions'
import TuningTab from "./TuningTab"
import PidsTab from "./PidsTab"

class TabContentContainer extends React.Component {
  render() {
    return (
      <div>
        {this.props.currentTab === 'GRAPHS' &&
          <TuningTab {...this.props.tuningTab} actions={this.props.actions} />
        }

        {this.props.currentTab === 'PIDS' &&
          <PidsTab {...this.props.pidsTab} actions={this.props.actions} />
        }

      </div>
    )
  }
}

export default connect(
  (state) => {
    const tabs = state.tabs

    return {
      currentTab: state.meta.currentTab,
      tuningTab: {
        graph1: tabs.graphs.graph1
      },
      pidsTab: {
      }
    }
  },

  (dispatch) => {
    return { actions: bindActionCreators(actions, dispatch) }
  }
)(TabContentContainer)
